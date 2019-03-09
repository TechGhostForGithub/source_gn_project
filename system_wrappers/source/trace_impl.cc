
#include "system_wrappers/source/trace_impl.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include "system_wrappers/source/trace_win.h"
#else
#include "system_wrappers/source/trace_posix.h"
#endif  // _WIN32

#include "system_wrappers/interface/sleep.h"

#define KEY_LEN_CHARS 31

#ifdef _WIN32
#pragma warning(disable:4355 4996)
#endif  // _WIN32

namespace gn
{

	const int Trace::kBoilerplateLength = 71;
	const int Trace::kTimestampPosition = 13;
	const int Trace::kTimestampLength = 12;
	uint32_t Trace::level_filter_ = kTraceDefault;

	// Construct On First Use idiom. Avoids "static initialization order fiasco".
	TraceImpl* TraceImpl::StaticInstance(CountOperation count_operation,
		const TraceLevel level)
	{
		// Sanities to avoid taking lock unless absolutely necessary (for
		// performance reasons). count_operation == kAddRefNoCreate implies that a
		// message will be written to file.
		if ((level != kTraceAll) && (count_operation == kAddRefNoCreate))
		{
			if (!(level & level_filter()))
			{
				return NULL;
			}
		}
		TraceImpl* impl =
			GetStaticInstance<TraceImpl>(count_operation);
		return impl;
	}

	TraceImpl* TraceImpl::GetTrace(const TraceLevel level)
	{
		return StaticInstance(kAddRefNoCreate, level);
	}

	TraceImpl* TraceImpl::CreateInstance()
	{
#if defined(_WIN32)
		return new TraceWindows();
#else
		return new TracePosix();
#endif
	}

	TraceImpl::TraceImpl()
		: critsect_interface_(CriticalSectionWrapper::CreateCriticalSection()),
		callback_(NULL),
		row_count_text_(0),
		file_count_text_(0),
		trace_file_(*FileWrapper::Create()),
		thread_(*ThreadWrapper::CreateThread(TraceImpl::Run, this,
		kHighestPriority, "Trace")),
		event_(*EventWrapper::Create()),
		critsect_array_(CriticalSectionWrapper::CreateCriticalSection()),
		next_free_idx_(),
		level_(),
		length_(),
		message_queue_(),
		active_queue_(0)
	{
		next_free_idx_[0] = 0;
		next_free_idx_[1] = 0;

		unsigned int tid = 0;
		thread_.Start(tid);

		for (int m = 0; m < GN_TRACE_NUM_ARRAY; ++m)
		{
			for (int n = 0; n < GN_TRACE_MAX_QUEUE; ++n)
			{
				message_queue_[m][n] = new
					char[GN_TRACE_MAX_MESSAGE_SIZE];
			}
		}
	}

	bool TraceImpl::StopThread()
	{
		// Release the worker thread so that it can flush any lingering messages.
		event_.Set();

		// Allow 10 ms for pending messages to be flushed out.
		// TODO(hellner): why not use condition variables to do this? Or let the
		//                worker thread die and let this thread flush remaining
		//                messages?
		SleepMs(10);

		thread_.SetNotAlive();
		// Make sure the thread finishes as quickly as possible (instead of having
		// to wait for the timeout).
		event_.Set();
		bool stopped = thread_.Stop();

		CriticalSectionScoped lock(critsect_interface_);
		trace_file_.Flush();
		trace_file_.CloseFile();
		return stopped;
	}

	TraceImpl::~TraceImpl()
	{
		StopThread();
		delete &event_;
		delete &trace_file_;
		delete &thread_;
		delete critsect_interface_;
		delete critsect_array_;

		for (int m = 0; m < GN_TRACE_NUM_ARRAY; ++m)
		{
			for (int n = 0; n < GN_TRACE_MAX_QUEUE; ++n)
			{
				delete [] message_queue_[m][n];
			}
		}
	}

	int32_t TraceImpl::AddThreadId(char* trace_message) const
	{
		uint32_t thread_id = ThreadWrapper::GetThreadId();
		// Messages is 12 characters.
		return sprintf(trace_message, "%10u; ", thread_id);
	}

	int32_t TraceImpl::AddLevel(char* sz_message, const TraceLevel level) const
	{
		const int kMessageLength = 12;
		switch (level)
		{
		case kTraceTerseInfo:
			// Add the appropriate amount of whitespace.
			memset(sz_message, ' ', kMessageLength);
			sz_message[kMessageLength] = '\0';
			break;
		case kTraceStateInfo:
			sprintf(sz_message, "STATEINFO ; ");
			break;
		case kTraceWarning:
			sprintf(sz_message, "WARNING   ; ");
			break;
		case kTraceError:
			sprintf(sz_message, "ERROR     ; ");
			break;
		case kTraceCritical:
			sprintf(sz_message, "CRITICAL  ; ");
			break;
		case kTraceInfo:
			sprintf(sz_message, "DEBUGINFO ; ");
			break;
		case kTraceModuleCall:
			sprintf(sz_message, "MODULECALL; ");
			break;
		case kTraceMemory:
			sprintf(sz_message, "MEMORY    ; ");
			break;
		case kTraceTimer:
			sprintf(sz_message, "TIMER     ; ");
			break;
		case kTraceStream:
			sprintf(sz_message, "STREAM    ; ");
			break;
		case kTraceApiCall:
			sprintf(sz_message, "APICALL   ; ");
			break;
		case kTraceDebug:
			sprintf(sz_message, "DEBUG     ; ");
			break;
		default:
			assert(false);
			return 0;
		}
		// All messages are 12 characters.
		return kMessageLength;
	}

	int32_t TraceImpl::AddModuleAndId(char* trace_message,
		const TraceModule module,
		const int32_t id) const
	{
		// Use long int to prevent problems with different definitions of
		// int32_t.
		// TODO(hellner): is this actually a problem? If so, it should be better to
		//                clean up int32_t
		const long int idl = id;
		const int kMessageLength = 27;
		if (idl != -1)
		{
			const unsigned long int id_engine = id >> 16;
			const unsigned long int id_channel = id & 0xffff;

			switch (module)
			{
			case kTraceUndefined:
				// Add the appropriate amount of whitespace.
				memset(trace_message, ' ', kMessageLength);
				trace_message[kMessageLength] = '\0';
				break;
			case kTraceNetworkService:
				sprintf(trace_message, "NETWORK SERVICE:%5ld %5ld;", id_engine,
					id_channel);
				break;
			case kTraceMessageSync:
				sprintf(trace_message, "   MESSAGE SYNC:%5ld %5ld;", id_engine,
					id_channel);
				break;
			case kTraceMessageKernel:
				sprintf(trace_message, " MESSAGE KERNEL:%5ld %5ld;", id_engine,
					id_channel);
				break;
			case kTraceClientKernel:
				sprintf(trace_message, "  CLIENT KERNEL:%5ld %5ld;", id_engine,
					id_channel);
				break;
			case kTraceFileModule:
				sprintf(trace_message, "    FILE MODELE:%5ld %5ld;", id_engine,
					id_channel);
				break;
			case kTraceFileTransfer:
				sprintf(trace_message, "  FILE TRANSFER:%5ld %5ld;", id_engine,
					id_channel);
				break;
			case kTraceUtility:
				sprintf(trace_message, "        UTILITY:%5ld %5ld;", id_engine,
					id_channel);
				break;
			case kTraceUserFace:
				sprintf(trace_message, "      USER FACE:%5ld %5ld;", id_engine,
					id_channel);
				break;
			case kTraceDataCenter:
				sprintf(trace_message, "    DATA CENTER:%5ld %5ld;", id_engine,
					id_channel);
				break;
			case kTraceImageKernel:
				sprintf(trace_message, "   IMAGE KERNEL:%5ld %5ld;", id_engine,
					id_channel);
				break;
			case kTraceEngineModule:
				sprintf(trace_message, "      ENGINE MODULE:%5ld %5ld;", id_engine,
					id_channel);
				break;
			case kTraceDataReport:
				sprintf(trace_message, "    DATA REPORT:%5ld %5ld;", id_engine,
					id_channel);
				break;
			case kTraceNetworkRouting:
				sprintf(trace_message, "NETWORK ROUTING:%5ld %5ld;", id_engine,
					id_channel);
				break;
			case kTraceCommonHead:
				sprintf(trace_message, "    COMMON HEAD:%5ld %5ld;", id_engine,
					id_channel);
				break;
            case kTraceHttpModule:
                sprintf(trace_message, "    HTTP MODULE:%5ld %5ld;", id_engine,
                    id_channel);
                break;
            case kTraceH5OfflinePkg:
                sprintf(trace_message, "     H5 PACKAGE:%5ld %5ld;", id_engine,
                    id_channel);
                break;
            case kTraceHttpKernel:
                sprintf(trace_message, "     HTTPKERNEL:%5ld %5ld;", id_engine,
                    id_channel);
                break;
			default:
                sprintf(trace_message, "               :%5ld %5ld;", id_engine,
                    id_channel);
				break;
			}
		}
		else
		{
			switch (module)
			{
			case kTraceUndefined:
				// Add the appropriate amount of whitespace.
				memset(trace_message, ' ', kMessageLength);
				trace_message[kMessageLength] = '\0';
				break;
			case kTraceNetworkService:
				sprintf(trace_message, "NETWORK SERVICE:%11ld;", idl);
				break;
			case kTraceMessageSync:
				sprintf(trace_message, "   MESSAGE SYNC:%11ld;", idl);
				break;
			case kTraceMessageKernel:
				sprintf(trace_message, " MESSAGE KERNEL:%11ld;", idl);
				break;
			case kTraceClientKernel:
				sprintf(trace_message, "  CLIENT KERNEL:%11ld;", idl);
				break;
			case kTraceFileModule:
				sprintf(trace_message, "    FILE MODELE:%11ld;", idl);
				break;
			case kTraceFileTransfer:
				sprintf(trace_message, "  FILE TRANSFER:%11ld;", idl);
				break;
			case kTraceUtility:
				sprintf(trace_message, "        UTILITY:%11ld;", idl);
				break;
			case kTraceUserFace:
				sprintf(trace_message, "      USER FACE:%11ld;", idl);
				break;
			case kTraceDataCenter:
				sprintf(trace_message, "    DATA CENTER:%11ld;", idl);
				break;
			case kTraceImageKernel:
				sprintf(trace_message, "   IMAGE KERNEL:%11ld;", idl);
				break;
			case kTraceEngineModule:
				sprintf(trace_message, "      AV Engine:%11ld;", idl);
				break;
			case kTraceDataReport:
				sprintf(trace_message, "    DATA REPORT:%11ld;", idl);
				break;
			case kTraceNetworkRouting:
				sprintf(trace_message, "NETWORK ROUTING:%11ld;", idl);
				break;
			case kTraceCommonHead:
				sprintf(trace_message, "    COMMON HEAD:%11ld;", idl);
				break;
            case kTraceHttpModule:
                sprintf(trace_message, "    HTTP MODULE:%11ld;", idl);
                break;
            case kTraceH5OfflinePkg:
                sprintf(trace_message, "     H5 PACKAGE:%11ld;", idl);
                break;
            case kTraceHttpKernel:
                sprintf(trace_message, "     HTTPKERNEL:%11ld;", idl);
                break;
			default:
                sprintf(trace_message, "               :%11ld;", idl);
				break;
			}
		}
		return kMessageLength;
	}

	int32_t TraceImpl::SetTraceFileImpl(const char* file_name_utf8,
		const bool add_file_counter)
	{
		CriticalSectionScoped lock(critsect_interface_);

		trace_file_.Flush();
		trace_file_.CloseFile();

		if (file_name_utf8)
		{
			if (add_file_counter)
			{
				file_count_text_ = 1;

				char file_name_with_counter_utf8[FileWrapper::kMaxFileNameSize];
				CreateFileName(file_name_utf8, file_name_with_counter_utf8,
					file_count_text_);
				if (trace_file_.OpenFile(file_name_with_counter_utf8, false, false,
					true) == -1)
				{
					return -1;
				}
			}
			else
			{
				file_count_text_ = 0;
				if (trace_file_.OpenFile(file_name_utf8, false, false, true) == -1)
				{
					return -1;
				}
			}
		}
		row_count_text_ = 0;
		return 0;
	}

	int32_t TraceImpl::TraceFileImpl(
		char file_name_utf8[FileWrapper::kMaxFileNameSize])
	{
		CriticalSectionScoped lock(critsect_interface_);
		return trace_file_.FileName(file_name_utf8, FileWrapper::kMaxFileNameSize);
	}

	int32_t TraceImpl::SetTraceCallbackImpl(TraceCallback* callback)
	{
		CriticalSectionScoped lock(critsect_interface_);
		callback_ = callback;
		return 0;
	}

	int32_t TraceImpl::AddMessage(
		char* trace_message,
		const char msg[GN_TRACE_MAX_MESSAGE_SIZE],
		const uint16_t written_so_far) const
	{
		int length = 0;
		if (written_so_far >= GN_TRACE_MAX_MESSAGE_SIZE)
		{
			return -1;
		}
		// - 2 to leave room for newline and NULL termination.
#ifdef _WIN32
		length = _snprintf(trace_message,
			GN_TRACE_MAX_MESSAGE_SIZE - written_so_far - 2,
			"%s", msg);
		if (length < 0)
		{
			length = GN_TRACE_MAX_MESSAGE_SIZE - written_so_far - 2;
			trace_message[length] = 0;
		}
#else
		length = snprintf(trace_message,
			GN_TRACE_MAX_MESSAGE_SIZE - written_so_far - 2,
			"%s", msg);
		if (length < 0 ||
			length > GN_TRACE_MAX_MESSAGE_SIZE - written_so_far - 2)
		{
			length = GN_TRACE_MAX_MESSAGE_SIZE - written_so_far - 2;
			trace_message[length] = 0;
		}
#endif
		// Length with NULL termination.
		return length + 1;
	}

	void TraceImpl::AddMessageToList(
		const char trace_message[GN_TRACE_MAX_MESSAGE_SIZE],
		const uint16_t length,
		const TraceLevel level)
	{
		// NOTE(andresp): Enabled externally.
#ifdef HJ_DIRECT_TRACE
		if (callback_)
		{
			callback_->Print(level, trace_message, length);
		}
		return;
#endif

		CriticalSectionScoped lock(critsect_array_);

		if (next_free_idx_[active_queue_] >= GN_TRACE_MAX_QUEUE)
		{
			if (!trace_file_.Open() && !callback_)
			{
				// Keep at least the last 1/4 of old messages when not logging.
				// TODO(hellner): isn't this redundant. The user will make it known
				//                when to start logging. Why keep messages before
				//                that?
				for (int n = 0; n < GN_TRACE_MAX_QUEUE / 4; ++n)
				{
					const int last_quarter_offset = (3 * GN_TRACE_MAX_QUEUE / 4);
					memcpy(message_queue_[active_queue_][n],
						message_queue_[active_queue_][n + last_quarter_offset],
						GN_TRACE_MAX_MESSAGE_SIZE);
				}
				next_free_idx_[active_queue_] = GN_TRACE_MAX_QUEUE / 4;
			}
			else
			{
				// More messages are being written than there is room for in the
				// buffer. Drop any new messages.
				// TODO(hellner): its probably better to drop old messages instead
				//                of new ones. One step further: if this happens
				//                it's due to writing faster than what can be
				//                processed. Maybe modify the filter at this point.
				//                E.g. turn of STREAM.
				return;
			}
		}

		uint16_t idx = next_free_idx_[active_queue_];
		next_free_idx_[active_queue_]++;

		level_[active_queue_][idx] = level;
		length_[active_queue_][idx] = length;
		memcpy(message_queue_[active_queue_][idx], trace_message, length);

		if (next_free_idx_[active_queue_] == GN_TRACE_MAX_QUEUE - 1)
		{
			// Logging more messages than can be worked off. Log a warning.
			const char warning_msg[] = "WARNING MISSING TRACE MESSAGES\n";
			level_[active_queue_][next_free_idx_[active_queue_]] = kTraceWarning;
			length_[active_queue_][next_free_idx_[active_queue_]] = strlen(warning_msg);
			memcpy(message_queue_[active_queue_][next_free_idx_[active_queue_]],
				warning_msg, strlen(warning_msg));
			next_free_idx_[active_queue_]++;
		}
	}

	bool TraceImpl::Run(void* obj)
	{
		return static_cast<TraceImpl*>(obj)->Process();
	}

	bool TraceImpl::Process()
	{
		if (event_.Wait(1000) == kEventSignaled)
		{
			// This slightly odd construction is to avoid locking |critsect_interface_|
			// while calling WriteToFile() since it's locked inside the function.
			critsect_interface_->Enter();
			bool write_to_file = trace_file_.Open() || callback_;
			critsect_interface_->Leave();
			if (write_to_file)
			{
				WriteToFile();
			}
		}
		else
		{
			CriticalSectionScoped lock(critsect_interface_);
			trace_file_.Flush();
		}
		return true;
	}

	void TraceImpl::WriteToFile()
	{
		uint8_t local_queue_active = 0;
		uint16_t local_next_free_idx = 0;

		// There are two buffers. One for reading (for writing to file) and one for
		// writing (for storing new messages). Let new messages be posted to the
		// unused buffer so that the current buffer can be flushed safely.
		{
			CriticalSectionScoped lock(critsect_array_);
			local_next_free_idx = next_free_idx_[active_queue_];
			next_free_idx_[active_queue_] = 0;
			local_queue_active = active_queue_;
			if (active_queue_ == 0)
			{
				active_queue_ = 1;
			}
			else 
			{
				active_queue_ = 0;
			}
		}
		if (local_next_free_idx == 0)
		{
			return;
		}

		CriticalSectionScoped lock(critsect_interface_);

		for (uint16_t idx = 0; idx < local_next_free_idx; ++idx) 
		{
			TraceLevel local_level = level_[local_queue_active][idx];
			if (callback_)
			{
				callback_->print(local_level, message_queue_[local_queue_active][idx],
					length_[local_queue_active][idx]);
			}
			if (trace_file_.Open())
			{
				if (row_count_text_ > GN_TRACE_MAX_FILE_SIZE)
				{
					// wrap file
					row_count_text_ = 0;
					trace_file_.Flush();

					if (file_count_text_ == 0)
					{
						trace_file_.Rewind();
					}
					else
					{
						char old_file_name[FileWrapper::kMaxFileNameSize];
						char new_file_name[FileWrapper::kMaxFileNameSize];

						// get current name
						trace_file_.FileName(old_file_name,
							FileWrapper::kMaxFileNameSize);
						trace_file_.CloseFile();

						file_count_text_++;

						UpdateFileName(old_file_name, new_file_name, file_count_text_);

						if (trace_file_.OpenFile(new_file_name, false, false,
							true) == -1)
						{
							return;
						}
					}
				}
				if (row_count_text_ ==  0)
				{
					char message[GN_TRACE_MAX_MESSAGE_SIZE + 1];
					int32_t length = AddDateTimeInfo(message);
					if (length != -1)
					{
						message[length] = 0;
						message[length - 1] = '\n';
						trace_file_.Write(message, length);
						row_count_text_++;
					}
					length = AddBuildInfo(message);
					if (length != -1)
					{
						message[length + 1] = 0;
						message[length] = '\n';
						message[length - 1] = '\n';
						trace_file_.Write(message, length + 1);
						row_count_text_++;
						row_count_text_++;
					}
				}
				uint16_t length = length_[local_queue_active][idx];
				message_queue_[local_queue_active][idx][length] = 0;
				message_queue_[local_queue_active][idx][length - 1] = '\n';
				trace_file_.Write(message_queue_[local_queue_active][idx], length);
				row_count_text_++;
			}
		}
	}

	void TraceImpl::AddImpl(const TraceLevel level, const TraceModule module,
		const int32_t id,
		const char msg[GN_TRACE_MAX_MESSAGE_SIZE])
	{
		if (TraceCheck(level))
		{
			char trace_message[GN_TRACE_MAX_MESSAGE_SIZE];
			char* message_ptr = trace_message;

			int32_t len = 0;
			int32_t ack_len = 0;

			len = AddLevel(message_ptr, level);
			if (len == -1) 
			{
				return;
			}
			message_ptr += len;
			ack_len += len;

			len = AddTime(message_ptr, level);
			if (len == -1)
			{
				return;
			}
			message_ptr += len;
			ack_len += len;

			len = AddModuleAndId(message_ptr, module, id);
			if (len == -1)
			{
				return;
			}
			message_ptr += len;
			ack_len += len;

			len = AddThreadId(message_ptr);
			if (len < 0)
			{
				return;
			}
			message_ptr += len;
			ack_len += len;

			len = AddMessage(message_ptr, msg, (uint16_t)ack_len);
			if (len == -1)
			{
				return;
			}
			ack_len += len;
			AddMessageToList(trace_message, (uint16_t)ack_len, level);

			// Make sure that messages are written as soon as possible.
			event_.Set();
		}
	}

	bool TraceImpl::TraceCheck(const TraceLevel level) const
	{
		return (level & level_filter()) ? true : false;
	}

	bool TraceImpl::UpdateFileName(
		const char file_name_utf8[FileWrapper::kMaxFileNameSize],
		char file_name_with_counter_utf8[FileWrapper::kMaxFileNameSize],
		const uint32_t new_count) const
	{
		int32_t length = (int32_t)strlen(file_name_utf8);
		if (length < 0)
		{
			return false;
		}

		int32_t length_without_file_ending = length - 1;
		while (length_without_file_ending > 0)
		{
			if (file_name_utf8[length_without_file_ending] == '.')
			{
				break;
			}
			else
			{
				length_without_file_ending--;
			}
		}
		if (length_without_file_ending == 0)
		{
			length_without_file_ending = length;
		}
		int32_t length_to_ = length_without_file_ending - 1;
		while (length_to_ > 0)
		{
			if (file_name_utf8[length_to_] == '_')
			{
				break;
			}
			else
			{
				length_to_--;
			}
		}

		memcpy(file_name_with_counter_utf8, file_name_utf8, length_to_);
		sprintf(file_name_with_counter_utf8 + length_to_, "_%lu%s",
			static_cast<long unsigned int>(new_count),
			file_name_utf8 + length_without_file_ending);
		return true;
	}

	bool TraceImpl::CreateFileName(
		const char file_name_utf8[FileWrapper::kMaxFileNameSize],
		char file_name_with_counter_utf8[FileWrapper::kMaxFileNameSize],
		const uint32_t new_count) const
	{
		int32_t length = (int32_t)strlen(file_name_utf8);
		if (length < 0)
		{
			return false;
		}

		int32_t length_without_file_ending = length - 1;
		while (length_without_file_ending > 0)
		{
			if (file_name_utf8[length_without_file_ending] == '.')
			{
				break;
			}
			else
			{
				length_without_file_ending--;
			}
		}
		if (length_without_file_ending == 0)
		{
			length_without_file_ending = length;
		}
		memcpy(file_name_with_counter_utf8, file_name_utf8,
			length_without_file_ending);
		sprintf(file_name_with_counter_utf8 + length_without_file_ending, "_%lu%s",
			static_cast<long unsigned int>(new_count),
			file_name_utf8 + length_without_file_ending);
		return true;
	}

	void Trace::CreateTrace()
	{
		TraceImpl::StaticInstance(kAddRef);
	}

	void Trace::ReturnTrace()
	{
		TraceImpl::StaticInstance(kRelease);
	}

	int32_t Trace::TraceFile(char file_name[FileWrapper::kMaxFileNameSize]) 
	{
		TraceImpl* trace = TraceImpl::GetTrace();
		if (trace)
		{
			int ret_val = trace->TraceFileImpl(file_name);
			ReturnTrace();
			return ret_val;
		}
		return -1;
	}

	int32_t Trace::SetTraceFile(const char* file_name,
		const bool add_file_counter)
	{
		TraceImpl* trace = TraceImpl::GetTrace();
		if (trace)
		{
			int ret_val = trace->SetTraceFileImpl(file_name, add_file_counter);
			ReturnTrace();
			return ret_val;
		}
		return -1;
	}

	int32_t Trace::SetTraceCallback(TraceCallback* callback)
	{
		TraceImpl* trace = TraceImpl::GetTrace();
		if (trace)
		{
			int ret_val = trace->SetTraceCallbackImpl(callback);
			ReturnTrace();
			return ret_val;
		}
		return -1;
	}

	void Trace::Add(const TraceLevel level, const TraceModule module,
		const int32_t id, const char* msg, ...)
	{
		TraceImpl* trace = TraceImpl::GetTrace(level);
		if (trace)
		{
			if (trace->TraceCheck(level))
			{
				char temp_buff[GN_TRACE_MAX_MESSAGE_SIZE];
				char* buff = 0;
				if (msg)
				{
					va_list args;
					va_start(args, msg);
#ifdef _WIN32
					_vsnprintf(temp_buff, GN_TRACE_MAX_MESSAGE_SIZE - 1, msg, args);
#else
					vsnprintf(temp_buff, GN_TRACE_MAX_MESSAGE_SIZE - 1, msg, args);
#endif
					va_end(args);
					buff = temp_buff;
				}
				trace->AddImpl(level, module, id, buff);
			}
			ReturnTrace();
		}
	}

}  // namespace gn

#ifdef _WIN32
#pragma warning(default:4355 4996)
#endif  // _WIN32

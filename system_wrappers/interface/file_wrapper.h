#ifndef GN_SYSTEM_WRAPPERS_INTERFACE_FILE_WRAPPER_H_
#define GN_SYSTEM_WRAPPERS_INTERFACE_FILE_WRAPPER_H_

#include <stddef.h>

#include "common_types.h"
#include "typedefs.h"

// Implementation of an InStream and OutStream that can read (exclusive) or
// write from/to a file.

namespace gn
{

	class FileWrapper : public InStream, public OutStream
	{
	public:
		static const size_t kMaxFileNameSize = 1024;

		// Factory method. Constructor disabled.
		static FileWrapper* Create();

		// Returns true if a file has been opened.
		virtual bool Open() const = 0;

		// Opens a file in read or write mode, decided by the read_only parameter.
		virtual int OpenFile(const char* file_name_utf8,
			bool read_only,
			bool loop = false,
			bool text = false) = 0;

		virtual int CloseFile() = 0;

		// Limits the file size to |bytes|. Writing will fail after the cap
		// is hit. Pass zero to use an unlimited size.
		virtual int SetMaxFileSize(size_t bytes)  = 0;

		// Flush any pending writes.
		virtual int Flush() = 0;

		// Returns the opened file's name in |file_name_utf8|. Provide the size of
		// the buffer in bytes in |size|. The name will be truncated if |size| is
		// too small.
		virtual int FileName(char* file_name_utf8,
			size_t size) const = 0;

		// Write |format| to the opened file. Arguments are taken in the same manner
		// as printf. That is, supply a format string containing text and
		// specifiers. Returns the number of characters written or -1 on error.
		virtual int WriteText(const char* format, ...) = 0;

		// Inherited from Instream.
		// Reads |length| bytes from file to |buf|. Returns the number of bytes read
		// or -1 on error.
		virtual int Read(void* buf, int length) = 0;

		// Inherited from OutStream.
		// Writes |length| bytes from |buf| to file. The actual writing may happen
		// some time later. Call Flush() to force a write.
		virtual bool Write(const void* buf, int length) = 0;

		// Inherited from both Instream and OutStream.
		// Rewinds the file to the start. Only available when OpenFile() has been
		// called with |loop| == true or |readOnly| == true.
		virtual int Rewind() = 0;
	};

}  // namespace gn

#endif  // GN_SYSTEM_WRAPPERS_INTERFACE_FILE_WRAPPER_H_

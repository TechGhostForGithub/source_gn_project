#ifndef _H_SQLITE_DATABASE_H
#define _H_SQLITE_DATABASE_H

#include <assert.h>
#include <vector>
#include "third_party/sqlite/source/string16.h"

#ifdef OS_ANDROID
// Use the system SQLite on Android.
#include <sqlite3.h>
#else
#include "third_party/sqlite/sqlite3i.h"
#endif

#define DISALLOW_EVIL_CONSTRUCTORS(TypeName)    \
	TypeName(const TypeName&);                    \
	void operator=(const TypeName&)

namespace gn
{
#if (defined(_DEBUG) || defined(DEBUG)) && defined(_MSC_VER)

#include <Windows.h>
#include <assert.h>
	class CurrentThreadID
	{
	public:
		CurrentThreadID()
		{
			id_ = GetCurrentThreadId();
		}
		DWORD get()
		{
			return id_;
		}
	private:
		DWORD id_;
	};

#define DECL_SINGLE_THREAD \
	CurrentThreadID thread_id_;

#define ASSERT_SINGLE_THREAD() \
	assert(thread_id_.get() == GetCurrentThreadId())

#else  // !DEBUG
#define DECL_SINGLE_THREAD
#define ASSERT_SINGLE_THREAD()
#endif
	//------------------------------------end fish
	class Mutex;
	class SQLStatement;

	void BuildSqliteErrorString(const char16 *summary, int sql_status, sqlite3 *db, std::string16 *out);

	class SQLTransactionListener
	{
	public:
		virtual void OnBegin() = 0;
		virtual void OnCommit() = 0;
		virtual void OnRollback() = 0;
	};

	class SQLDatabase
	{
	public:
		SQLDatabase();
		~SQLDatabase();
		bool Open(const char16 *name);
		bool IsOpen();
		void Close();
		int Execute(const char *sql);
		int GetErrorCode();
		const char *GetErrorMessage();
		bool IsInTransaction();
		bool BeginTransaction(const char *log_label);
		void RollbackTransaction(const char *log_label);
		bool CommitTransaction(const char *log_label);
		void SetTransactionMutex(Mutex *mutex);
		void SetTransactionListener(SQLTransactionListener *listener);
		bool DropAllObjects();
		static const char* kUnspecifiedTransactionLabel;
		static const int kBusyTimeout = 5 * 1000;

	private:
		DECL_SINGLE_THREAD;

		friend class SQLStatement;
		sqlite3 *GetDBHandle();
		friend bool TestSQLConcurrency();
		bool OpenConnection(const char16 *name);
		bool ConfigureConnection();
		bool EndTransaction(const char *log_label);

		sqlite3 *db_;
		int transaction_count_;
		bool needs_rollback_;
		int64_t transaction_start_time_; 
		Mutex* opt_transaction_mutex_;
		// callbacks
		SQLTransactionListener *transaction_listener_;
	};
}
#endif

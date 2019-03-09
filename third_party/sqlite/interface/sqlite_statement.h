#ifndef H_SQLITE_STATEMENT_H
#define H_SQLITE_STATEMENT_H

#include "third_party/sqlite/sqlite3.h"
#include "third_party/sqlite/interface/sqlite_database.h"

namespace gn
{
	class SQLDatabase;

	class scoped_sqlite3_stmt_ptr 
	{
	public:
		scoped_sqlite3_stmt_ptr() ;
		explicit scoped_sqlite3_stmt_ptr(sqlite3_stmt *stmt);
		~scoped_sqlite3_stmt_ptr();

		sqlite3_stmt *get() const ;
		void set(sqlite3_stmt *stmt) ;
		sqlite3_stmt *release() ;
		sqlite3_stmt** operator &() ;
		int finalize() ;

	protected:
		sqlite3_stmt *stmt_;

	private:
		DISALLOW_EVIL_CONSTRUCTORS(scoped_sqlite3_stmt_ptr);
	};

	class SQLStatement : public scoped_sqlite3_stmt_ptr
	{
		typedef void (*Function)(void*);
	public:
		SQLStatement();

		int prepare16(SQLDatabase *db, const char16 *sql);
		int prepare16(SQLDatabase *db, const char16 *sql, int sql_len);
		bool nextrow();
		bool executesql(SQLDatabase *db, const char16 *sql);
		int step();
		int reset();
		sqlite_int64 last_insert_rowid();
		sqlite3 *db_handle();
		int bind_parameter_count();
		int bind_blob(int index, std::vector<uint8_t> *blob);
		int bind_blob(int index, const void *value, int value_len);
		int bind_blob(int index, const void *value, int value_len, Function dtor);
		int bind_double(int index, double value);
		int bind_int(int index, int value);
		int bind_int64(int index, sqlite_int64 value);
		int bind_null(int index);
		int bind_text16(int index, const char16 *value);
		int bind_text16(int index, const char16 *value, int value_len);
		int bind_text16(int index, const char16 *value, int value_len,Function dtor);
		int bind_value(int index, const sqlite3_value *value);
		int column_count() ;
		int column_type(int index);
		const char16 *column_name16(int index) ;
		const void *column_blob(int index);
		bool column_blob_as_vector(int index, std::vector<uint8_t> *blob);
		int column_bytes(int index);
		int column_bytes16(int index);
		double column_double(int index) ;
		int column_int(int index);
		sqlite_int64 column_int64(int index) ;
		// Returns the text value for this column or NULL if no value has been set
		const char16 *column_text16(int index);
		// Returns the text value for this column or an empty string if no value
		// has been set
		const char16 *column_text16_safe(int index) ;
	};

}
#endif

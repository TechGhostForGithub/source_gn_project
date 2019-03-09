#include "third_party/sqlite/source/statement.h"
#include "third_party/sqlite/source/sqlite.h"

namespace gn
{
	CStatement::CStatement()
		: stmt_ (0)
	{
	}

	CStatement::~CStatement()
	{
		if(stmt_)
		{
			sqlite3_finalize(stmt_);
		}
	}

	int32_t CStatement::init(const char* statement, sqlite3* db)
	{
		if (sqlite3_prepare(db,statement,-1,&stmt_,0) != SQLITE_OK)
		{
			return -1;
		}

		if (!stmt_)
		{
			return -1;
		}

		return 0;
	}

	int32_t CStatement::Bind(int32_t pos_zero_indexed, std::string const& value)
	{
		if (sqlite3_bind_text(stmt_,pos_zero_indexed + 1,value.c_str(),
			value.length(),// length of text
			SQLITE_TRANSIENT // SQLITE_TRANSIENT: SQLite makes its own copy
			)
			!= SQLITE_OK)
		{
			return -1;
		}
		return 0;
	}

	int32_t CStatement::Bind(int32_t pos_zero_indexed, double value)
	{
		if (sqlite3_bind_double(
			stmt_,
			pos_zero_indexed+1,// Index of wildcard
			value
			)
			!= SQLITE_OK)
		{
			return -1;
		}
		return 0;
	}

	int32_t CStatement::Bind(int32_t pos_zero_indexed, int32_t value)
	{
		if (sqlite3_bind_int(
			stmt_,
			pos_zero_indexed+1,// Index of wildcard
			value 
			)
			!= SQLITE_OK)
		{
			return -1;
		}
		return 0;
	}

	int32_t CStatement::BindNull(int32_t pos_zero_indexed)
	{
		if (sqlite3_bind_null(
			stmt_,
			pos_zero_indexed+1// Index of wildcard
			)
			!= SQLITE_OK)
		{
			return -1;
		}
		return 0;
	}

	int32_t CStatement::Execute()
	{
		int32_t rc = sqlite3_step(stmt_);
		if (rc == SQLITE_BUSY
			|| rc == SQLITE_ERROR
			|| rc == SQLITE_MISUSE
			|| rc != SQLITE_DONE)
		{
			return -1;
		}
		sqlite3_reset(stmt_);
		return 0;
	}

	CStatement::dataType CStatement::DataType(int32_t pos_zero_indexed)
	{
		return dataType(sqlite3_column_type(stmt_, pos_zero_indexed));
	}

	int32_t CStatement::ValueInt(int32_t pos_zero_indexed)
	{
		return sqlite3_column_int(stmt_, pos_zero_indexed);
	}

	std::string CStatement::ValueString(int32_t pos_zero_indexed)
	{
		return std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt_, pos_zero_indexed)));
	}

	int32_t CStatement::RestartSelect()
	{
		sqlite3_reset(stmt_);
		return 0;
	}

	int32_t CStatement::Reset()
	{
		int32_t rc = sqlite3_step(stmt_);

		sqlite3_reset(stmt_);

		if (rc == SQLITE_ROW)
		{
			return 0;
		}
		return -1;
	}

	int32_t CStatement::NextRow()
	{
		int32_t rc = sqlite3_step(stmt_);

		if (rc == SQLITE_ROW )
		{
			return 0;
		}

		if (rc == SQLITE_DONE)
		{
			sqlite3_reset(stmt_);
			return -1;
		}

		return -1;
	}
}

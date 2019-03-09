#ifndef _STATEMENT_H_
#define _STATEMENT_H_
#include <vector>
#include <string>
#include "common_types.h"
#include "third_party/sqlite/sqlite3i.h"
namespace gn
{
	class CSqlite;
	class CStatement
	{
	private:
		friend class CSqlite;
		int32_t init(const char* statement, sqlite3* db);

	public:
		CStatement();
		virtual~CStatement();

		enum dataType
		{
			INT = SQLITE_INTEGER,
			FLT = SQLITE_FLOAT,
			TXT = SQLITE_TEXT ,
			BLB = SQLITE_BLOB ,
			NUL = SQLITE_NULL ,
		};

		dataType DataType(int32_t pos_zero_indexed);

		int32_t ValueInt (int32_t pos_zero_indexed);
		std::string ValueString(int32_t pos_zero_indexed);

		int32_t Bind(int32_t pos_zero_indexed, std::string const& value);
		int32_t Bind(int32_t pos_zero_indexed, double value);
		int32_t Bind(int32_t pos_zero_indexed, int32_t value);
		int32_t BindNull(int32_t pos_zero_indexed);

		int32_t Execute();

		int32_t NextRow();

		/* Call Reset if not depending on the NextRow cleaning up. 
		For example for select count(*) statements*/
		int32_t Reset();

		int32_t RestartSelect();

	private:
		sqlite3_stmt* stmt_;
	};
}
#endif // _STATEMENT_H_

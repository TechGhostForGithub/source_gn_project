#ifndef _SQLITE_H_
#define _SQLITE_H_
#include <vector>
#include <string>
#include "common_types.h"
#include "third_party/sqlite/sqlite3i.h"
#include "third_party/sqlite/source/statement.h"
#include "system_wrappers/interface/atomic32.h"
namespace gn
{
	struct ResultRecord
	{
		std::vector<std::string> fields_;
	};

	class ResultTable
	{
		friend class CSqlite;
	public:
		ResultTable();
		ResultRecord* next();
	private:
		void reset();
	private:
		std::vector<ResultRecord> records_;
		uint32_t ptr_cur_record_;
	};

	class CSqlite
	{
	public:
		CSqlite();
		virtual ~CSqlite();
		int32_t Open(const char* db_file);
		int32_t Close();
		int32_t IsOpen();

		int32_t SelectStmt (const char* stmt, ResultTable& res);
		int32_t DirectStatement(const char* stmt);
		CStatement* BuildStmt(const char* sql);
		void ReleaseStmt(CStatement* stmt);

		std::string LastError();

		// Transaction related
		int32_t Begin ();
		int32_t Commit();
		int32_t Rollback();

	private:
		static int32_t SelectCallback(
			void *p_data,
			int32_t num_fields, 
			char **p_fields, 
			char **p_col_names);

	private:
		sqlite3*	db_;
		Atomic32	transaction_count_;
	};
}
#endif // _SQLITE_H_

#include "sqlite.h"
#include <assert.h>
namespace gn
{
	ResultTable::ResultTable()
		: ptr_cur_record_(0)
	{
	}

	ResultRecord* ResultTable::next()
	{
		if (ptr_cur_record_ < records_.size())
		{
			return &(records_[ptr_cur_record_++]);
		}
		return 0;
	}

	void ResultTable::reset()
	{
		records_.clear();
		ptr_cur_record_ = 0;
	}

	//-----------------------------------------------------------
	CSqlite::CSqlite()
		: db_(0)
		, transaction_count_(0)
	{
	}

	CSqlite::~CSqlite()
	{
		assert(db_ == 0);
		assert(transaction_count_.Value() == 0);
	}

	int32_t CSqlite::Open(const char* db_file)
	{
		if (db_)
		{
			return -1;
		}
		if (sqlite3_open(db_file, &db_) != SQLITE_OK)
		{
			return -1;
		}
		return 0;
	}

	int32_t CSqlite::Close()
	{
		if (!db_)
		{
			return -1;
		}
		if (sqlite3_close(db_) != SQLITE_OK)
		{
			return -1;
		}  

		db_ = 0;
		return 0;
	}

	int32_t CSqlite::IsOpen()
	{
		return db_ == 0 ? -1 : 0;
	}

	int32_t CSqlite::SelectStmt(const char* stmt, ResultTable& res)
	{
		res.reset();

		int32_t ret = sqlite3_exec(db_, stmt, SelectCallback, static_cast<void*> (&res), 0);
		if (ret != SQLITE_OK)
		{
			return -1;
		}
		return 0;
	}

	int32_t CSqlite::SelectCallback(
		void *p_data, 
		int32_t num_fields, 
		char **p_fields, 
		char** p_col_names)
	{
		ResultTable* res = reinterpret_cast<ResultTable*>(p_data);
		ResultRecord record;
		for(int32_t i = 0; i < num_fields; i++)
		{
			// Hubert Castelain: special treatment if null
			if (p_fields[i])
			{
				record.fields_.push_back(p_fields[i]);
			}
			else
			{
				record.fields_.push_back("<null>");
			}
		}

		res->records_.push_back(record);

		return 0;
	}

	CStatement* CSqlite::BuildStmt(const char* sql)
	{
		CStatement* stmt = new CStatement();
		if (stmt->init(sql, db_) == -1)
		{
			delete stmt;
			stmt = 0;
		}
		return stmt;
	}

	void CSqlite::ReleaseStmt(CStatement* stmt)
	{
		if (stmt)
		{
			delete stmt;
		}
	}

	int32_t CSqlite::DirectStatement(const char* stmt)
	{
		int32_t ret = sqlite3_exec(db_, stmt, 0, 0, 0);

		if(ret != SQLITE_OK)
		{
			return -1;
		}
		return 0;
	}

	std::string CSqlite::LastError()
	{
		return sqlite3_errmsg(db_);
	}

	int32_t CSqlite::Begin()
	{
		int32_t ret = 0;
		if (++transaction_count_ == 1)
		{
			ret = DirectStatement("begin immediate");
			if (ret != 0)
			{
				--transaction_count_;
				return ret;
			}
		}
		return ret;
	}

	int32_t CSqlite::Commit()
	{
		if (transaction_count_.Value() > 0)
		{
			--transaction_count_;
			if (transaction_count_.Value() == 0)
			{
				return DirectStatement("commit");
			}
			return 0;
		}
		return -1;
	}

	int32_t CSqlite::Rollback()
	{
		if (transaction_count_.Value() > 0)
		{
			--transaction_count_;
			if (transaction_count_.Value() == 0)
			{
				return DirectStatement("rollback");
			}
			return 0;
		}
		return -1;
	}
}

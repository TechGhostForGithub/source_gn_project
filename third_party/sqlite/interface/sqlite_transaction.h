#ifndef H_SQLITE_TRANSACTION_H
#define  H_SQLITE_TRANSACTION_H
#include "sqlite_database.h"

namespace gn
{
	class SQLTransaction
	{
	public:
		SQLTransaction(SQLDatabase *db, const char *log_label);
		~SQLTransaction();

		bool Begin();
		bool BeginIfNeeded();
		bool Commit();
		void Rollback();
		bool HasBegun();

	private:
		DISALLOW_EVIL_CONSTRUCTORS(SQLTransaction);

		bool End(bool commit);

	private:
		bool			began_;
		SQLDatabase*	db_;
		std::string		log_label_;
	};

}
#endif

#include"DataManager.h"
#include"Sysutil.h"

SqliteManager::SqliteManager() : m_db(NULL)
{
}
SqliteManager::~SqliteManager()
{

}
void  SqliteManager::Open(const string &path)
{
	char *zErrMsg = 0;
	int rc = sqlite3_open(path.c_str(), &m_db);
	if (rc != SQLITE_OK)
	{
		ERROR_LOG("Open databases failed %s\n", sqlite3_errmsg(m_db));
		exit(0);
	}
	else
	{
		TRACE_LOG("Open databases Successfully \n");
	} 
}
void SqliteManager::Close()
{
	if (m_db)
	{
		int rc = sqlite3_close(m_db);
		if (rc != SQLITE_OK)
		{
			ERROR_LOG("Close database failed: %s\n", sqlite3_errmsg(m_db));
		}
		else
		{
			TRACE_LOG("Close database successed\n");
		}
	}
}
void SqliteManager::ExecuteSql(const string &sql)
{
	char *zErrMsg = 0;
	int rc = sqlite3_exec(m_db, sql.c_str(), 0, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		ERROR_LOG("SQL(%s) Error: %s\n",sql.c_str(),sqlite3_errmsg(m_db));
		sqlite3_free(zErrMsg);
	}
	else
	{
		ERROR_LOG("SQL(%s) Successfully %s \n",sql.c_str(),zErrMsg);
	}

}

void SqliteManager::GetResultTable(const string &sql, int &row, int &col, char** &ppRet)
{
	char *zErrMsg = 0;
	int rc = sqlite3_get_table(m_db, sql.c_str(), &ppRet, &row, &col, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		ERROR_LOG("Get Result Table failed : %s\n", zErrMsg);
		exit(0);
	}
	else
	{
		TRACE_LOG("Get Result Table successfully %s \n", zErrMsg);
	}
}

#pragma once

#include"common.h"
#include"sqlite3.h"

class SqliteManager
{
private:
	sqlite3 *m_db;
public:
	SqliteManager();
	~SqliteManager();
public:
	void Open(const string &path);
	void Close();
	void ExecuteSql(const string &sql);
	void GetResultTable(const string &sql,int& row,int& col,char** &ppRet);
};

class DataManager
{
private:
	SqliteManager m_dbmgr;
public:
	DataManager();
	~DataManager();
public:
	void InsertDoc(const string &path, string &doc);
	void GetDocs(const string &path, set<string> &docs);
	void DeleteDoc(const string& path, string doc);
public:
	void InitSqlite();

};


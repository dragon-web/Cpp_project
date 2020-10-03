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
	void InsertDoc(const string &path, const string &doc);
	void GetDocs(const string &path, multiset<string> &docs);
	void DeleteDoc(const string& path,  const string &doc);
public:
	void InitSqlite();
public:
	void Search(const string &key, vector<pair<string, string>> &doc_path);
};

class AutoGetResultTable
{
public:
	AutoGetResultTable(SqliteManager *db, const string &sql, int &col, int &row,char **& ppRet);
	~AutoGetResultTable();
public:
	AutoGetResultTable(const AutoGetResultTable &) = delete;
	AutoGetResultTable& operator = (const AutoGetResultTable &) = delete;
private:
	SqliteManager* m_db;
	char **m_ppRet;
};

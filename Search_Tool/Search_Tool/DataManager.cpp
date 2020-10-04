#include"DataManager.h"
#include"Sysutil.h"
#include"Logal_Func.h"

SqliteManager::SqliteManager() : m_db(NULL)
{}
SqliteManager::~SqliteManager()
{

}

DataManager &DataManager::GetInstance()
{
	static DataManager inst;
	return inst;
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
		//exit(0);
	}
	else
	{
		TRACE_LOG("Get Result Table successfully %s \n", zErrMsg);
	}
}


#define DOC_DB "doc.db"
#define DOC_TABLE "doc_tb"  


DataManager::DataManager()
{
	m_dbmgr.Open(DOC_DB);
	InitSqlite();
}
void DataManager::DeleteDoc(const string& path,const string &doc)
{
	char sql[SQ_BUFFER_SIZE] = { 0 };
	sprintf(sql, "delete from %s where doc_name = '%s' and doc_path = '%s'", DOC_TABLE, doc.c_str(), path.c_str());
	m_dbmgr.ExecuteSql(sql);

	//如果删除的是目录文件，则删除对应目录下的子文件
	//采取模糊匹配
	string doc_path = path;
	doc_path += "\\";
	doc_path += doc;
	memset(sql, 0, SQ_BUFFER_SIZE);
	sprintf(sql, "delete from %s where doc_path like '%s%%'",DOC_TABLE,doc_path.c_str());
	m_dbmgr.ExecuteSql(sql);
}

void DataManager::GetDocs(const string &path, multiset<string> &docs)
{
	char sql[SQ_BUFFER_SIZE] = { 0 };
	sprintf(sql, "select doc_name from %s where doc_path = '%s'", DOC_TABLE,path.c_str());
	int row = 0, col = 0;
	char** ppRet = 0;
	m_dbmgr.GetResultTable(sql,row,col,ppRet);
	//AutoGetResultTable(&m_dbmgr,sql, row, col, ppRet);

	for (int i = 1; i <= row; ++i)
	{
		docs.insert(ppRet[i]);
	}
	//释放结果表
	sqlite3_free_table(ppRet);
}
void DataManager::InsertDoc(const string &path, const string &doc)
{
	char sql[SQ_BUFFER_SIZE] = { 0 };
	string pinyin = ChineseConvertPinYinAllSpell(doc);
	string initials = ChineseConvertPinYinInitials(doc);
	sprintf(sql,"insert into %s values(null,'%s','%s','%s','%s')",
		DOC_TABLE,doc.c_str(), path.c_str(),pinyin.c_str(),initials.c_str());
	m_dbmgr.ExecuteSql(sql);
}
DataManager::~DataManager()
{

}
void DataManager::InitSqlite()
{
	char sql[SQ_BUFFER_SIZE] = { 0 };
	sprintf(sql,
		"create table if not exists %s(id integer primary key autoincrement, doc_name text, doc_path text,doc_pinyin text,doc_initials text)",
		DOC_TABLE);
	m_dbmgr.ExecuteSql(sql);
}

void DataManager::Search(const string &key, vector<pair<string, string>> &doc_path)
{
	char sql[SQ_BUFFER_SIZE] = { 0 };
	//sprintf(sql, "select doc_name,doc_path from %s where doc_name like '%%%s%%'",DOC_TABLE,key.c_str());
	string pinyin = ChineseConvertPinYinAllSpell(key);
	string initials = ChineseConvertPinYinInitials(key);
	sprintf(sql, "select doc_name,doc_path from %s where doc_pinyin like '%%%s%%' or doc_initials like '%%%s%%'", 
		DOC_TABLE, pinyin.c_str(),initials.c_str());
	int row = 0;
	int col = 0;
	char **ppRet = nullptr;
	//m_dbmgr.GetResultTable(sql, row, col, ppRet);
	AutoGetResultTable at(&m_dbmgr,sql,row,col,ppRet);  //这里的at是对象
	for (int i = 1; i <= row; ++i)
	{
		doc_path.push_back(make_pair(ppRet[i*col], ppRet[i*col + 1]));
	}

	//释放结果表
	//sqlite3_free_table(ppRet);
}

//RAII机制：资源获取就是初始化

AutoGetResultTable::AutoGetResultTable(SqliteManager *db, const string &sql, 
	int &col, int &row, char **& ppRet) : m_db(db)
{
	m_db->GetResultTable(sql, row, col, ppRet);
	m_ppRet = ppRet;
}
AutoGetResultTable::~AutoGetResultTable()
{
	if (m_ppRet)
		sqlite3_free_table(m_ppRet);
}


void DataManager::SplitHighlight(const string &str, const string &key,
	string &prefix, string &highlight, string &suffix)
{
	string strlower(str), keylower(key);
	transform(strlower.begin(), strlower.end(), strlower.begin(), ::tolower);
	transform(keylower.begin(), keylower.end(), keylower.begin(), ::tolower);

	//1 如果中文搜索，并能搜索成功，则直接分离
	size_t pos = strlower.find(keylower);
	if (pos != string::npos)
	{
		prefix = str.substr(0, pos);
		highlight = str.substr(pos, keylower.size());
		suffix = str.substr(pos + keylower.size(), string::npos);
		return;
	}

	//2 使用拼音全拼搜索, 则需要匹配分离子串汉字和拼音
	string str_pinyin = ChineseConvertPinYinAllSpell(strlower);
	string key_pinyin = ChineseConvertPinYinAllSpell(keylower);
	pos = str_pinyin.find(key_pinyin);
	if (pos != string::npos)
	{
		size_t str_index = 0;
		size_t pinyin_index = 0;

		size_t highlight_index = 0;
		size_t highlight_len = 0;

		while (str_index < strlower.size())
		{
			if (pinyin_index == pos)
			{
				highlight_index = str_index;
			}
			if (pinyin_index >= pos + key_pinyin.size())
			{
				highlight_len = str_index - highlight_index;
				break;
			}
			if (strlower[str_index] >= 0 && strlower[str_index] <= 127)
			{
				//是一个字符
				++str_index;
				++pinyin_index;
			}
			else
			{
				//是一个汉字
				string word(strlower, str_index, 2);  //文
				string word_pinyin = ChineseConvertPinYinAllSpell(word); //wen

				str_index += 2;
				pinyin_index += word_pinyin.size();
			}
		}
		prefix = str.substr(0, highlight_index);
		highlight = str.substr(highlight_index, highlight_len);
		suffix = str.substr(highlight_index + highlight_len, string::npos);
		return;
	}

	//3 使用首字母搜索
	string str_initials = ChineseConvertPinYinInitials(strlower);
	string key_initials = ChineseConvertPinYinInitials(keylower);
	pos = str_initials.find(key_initials);
	if (pos != string::npos)
	{
		size_t str_index = 0;
		size_t initials_index = 0;

		size_t highlight_index = 0;
		size_t highlight_len = 0;

		while (str_index < strlower.size())
		{
			if (initials_index == pos)
			{
				highlight_index = str_index;
			}
			if (initials_index >= pos + key_initials.size())
			{
				highlight_len = str_index - highlight_index;
				break;
			}
			if (strlower[str_index] >= 0 && strlower[str_index] <= 127)
			{
				//是一个字符
				++str_index;
				++initials_index;
			}
			else
			{
				//是一个汉字
				str_index += 2;
				initials_index++;
			}
		}

		prefix = str.substr(0, highlight_index);
		highlight = str.substr(highlight_index, highlight_len);
		suffix = str.substr(highlight_index + highlight_len, string::npos);
		return;
	}


	prefix = str;
	highlight.clear();
	suffix.clear();
}
void ColourPrintf(const char* str)
{
	// 0-黑 1-蓝 2-绿 3-浅绿 4-红 5-紫 6-黄 7-白 8-灰 9-淡蓝 10-淡绿
	// 11-淡浅绿 12-淡红 13-淡紫 14-淡黄 15-亮白
	//颜色：前景色 + 背景色*0x10
	//例如：字是红色，背景色是白色，即 红色 + 亮白 = 4 + 15*0x10
	//WORD color = 4 + 15 * 0x10;

	WORD color = 11 + 0 * 0x10;
	WORD colorOld;
	HANDLE handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(handle, &csbi);
	colorOld = csbi.wAttributes;
	SetConsoleTextAttribute(handle, color);
	printf("%s", str);
	SetConsoleTextAttribute(handle, colorOld);
}
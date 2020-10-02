#include"Sysutil.h"
#include"sqlite3.h"
#include"DataManager.h"
#include"ScanManager.h"

//静态库形式或者是源码形式调用，在程序里面使用数据库

#pragma comment(lib,"C:\\Users\\飞龙\\Desktop\\Cpp_project\\Search_Tool\\Search_Tool\\sqlite3_lib\\sqlite3.lib")

void Test_Manager()
{
	SqliteManager sm;
	sm.Open("doc.db");
	//string sql = "create table if not exists doc_tb(id integer primary key autoincrement, doc_name text, doc_path text)";
	//sm.ExecuteSql(sql);
	//string sql1 = "insert into doc_tb values(null,'stl.pdf','c:\\')";
	//sm.ExecuteSql(sql1);
	string sql = "select * from doc_tb";
	int row = 0;
	int col = 0;
	char **ppRet = 0;
	sm.GetResultTable(sql, row, col, ppRet);
	for (int i = 0; i <= row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			printf("%-10s", *(ppRet + (i*col) + j));
		}
		printf("\n");
	}
	sm.Close();
	sqlite3_free_table(ppRet);
}

void Test_Log()
{
	FILE* fp = fopen("Test.txt", "r");
	if (fp == NULL)
	{
		TRACE_LOG("Open File Error.");
		return;
	}
	else
	{
		TRACE_LOG("Open File Success");
	}
	fclose(fp);
}


void Test_MapSet()
{
	vector<int> v = { 5,8,9,3,7,6,2,1,0 };
	set<int> s;
	for ( const auto &e : v)
	{
		s.insert(e);
	}
	s.erase(9);
	for ( const auto &e : s)
	{
		cout << e << " ";
	}
}


void Test_Scan()
{
	const string &path = "C:\\Users\\飞龙\\Desktop\\review_cpp\\Problem2";
	ScanManager sm;
	sm.ScanDirectory(path);

}


void DirectionList(const string &path, vector<string> &subfile, vector<string> &subdir)
{
	string _path = path;  //"C:\\Users\\baoso\\Desktop\\Pro_81\\my_dir"
	_path += "\\*.*";     //"C:\\Users\\baoso\\Desktop\\Pro_81\\my_dir\\*.*";

	_finddata_t file;
	long handle = _findfirst(_path.c_str(), &file); //搜索path路劲下的第一个文件
	if (handle == -1)
	{
		perror("_findfirst");  //日志系统
		return;
	}

	do
	{
		if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0) //skip . and ..
			continue;

		if (file.attrib & _A_SUBDIR)
		{
			//目录
			//cout<<file.name<<endl;
			//string tmp_path = path; //"C:\\Users\\baoso\\Desktop\\Pro_81\\my_dir";
			//tmp_path += "\\";       //"C:\\Users\\baoso\\Desktop\\Pro_81\\my_dir\\";
			//tmp_path += file.name;  //"C:\\Users\\baoso\\Desktop\\Pro_81\\my_dir\\AA";
			//DirectionList(tmp_path);
			subdir.push_back(file.name);
		}
		else
			//cout<<file.name<<endl;
			subfile.push_back(file.name);
	} while (_findnext(handle, &file) == 0);

	_findclose(handle);
}


int main()
{
	//Test_Sqlite();
	//Test_Manager();
	//Test_Log();
	//Test_MapSet();
	Test_Scan();
	system("pause");
	return 0;
}

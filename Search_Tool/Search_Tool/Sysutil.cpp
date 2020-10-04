#include"Sysutil.h"
#include"sqlite3.h"
#include"DataManager.h"
#include"ScanManager.h"
#include"Logal_Func.h"
#include"Sysframe.h"

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

/*
void Test_Scan()
{
	const string &path = "C:\\Users\\飞龙\\Desktop\\Test_Pro";
	ScanManager sm;
	sm.ScanDirectory(path); 
}
*/


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


void Test_Search()
{
	//扫描搜索实例
	const string &path = "C:\\Users\\飞龙\\Desktop\\Test_Pro";
	ScanManager::CreateInstance(path).ScanDirectory(path);
	//sm.ScanDirectory(path);

	//创建搜索实例
	DataManager &dm = DataManager::GetInstance();
	string key;
	vector<pair<string, string>> doc_path;
	while (1)
	{
		cout << "Print:" << endl;
		cin >> key;
		dm.Search(key,doc_path);
		printf("%-15s%-15s\n", "名称","路径");
		for (const auto &e : doc_path)
		{
			printf("%-10s%-50s\n", e.first.c_str(), e.second.c_str());
		}
		doc_path.clear();
	}
}
/*
void thread_fun() 
{
	for (int i = 0; i < 10; ++i)
	{
		cout << "This is a Child Thread" << endl;
	}
}

void Test_thread()
{
	thread th(thread_fun);
	th.detach();//主线程和子线程分离
	for (int i = 0; i < 10; ++i)
	{
		cout << "This is a Main Thread" << endl;
	}
	th.join();
}

class Test
{
public:
	void fun()
	{
		cout << "This is Test::fun()" << endl;
	}
};

void Test_thread()
{
	Test t;
	thread th(&Test::fun,&t);
	th.detach();//主线程和子线程分离
	for (int i = 0; i < 10; ++i)
	{
		cout << "This is a Main Thread" << endl;
	}
	//th.join();
}
*/
/*
void Test_Frame()
{
	/*system("mode con cols = 60 lines = 10");
	SetCurPros(0,(60-strlen("Hello Bit.")) / 2);
	printf("Hello Bit.\n");
	
	DrawFrame(title);
}

int main()
{
	//Test_Sqlite();
	//Test_Manager();
	//Test_Log();
	//Test_MapSet();
	//Test_Search();
	//Test_thread();
	Test_Frame();
	system("pause");
	return 0;
}*/

const char* Title = "文档快速搜索工具";

int main()
{
	const string &path = "C:\\Users\\飞龙\\Desktop\\Test_Pro";
	ScanManager::CreateInstance(path).ScanDirectory(path);
	//sm.ScanDirectory(path);

	//创建搜索实例
	DataManager &dm = DataManager::GetInstance();
	string key;
	vector<pair<string, string>> doc_path;
	while (1)
	{
		DrawFrame(Title);
		DrawMenu();
		cin >> key;
		if (key == "exit")
			break;
		dm.Search(key, doc_path);
		int init_row = 5;
		int count = 0;
		string prefix, highlight, suffix;
		for (const auto& e : doc_path)//打印搜到的内容
		{
			string doc_name = e.first;
			string doc_path = e.second;
			DataManager::SplitHighlight(doc_name, key, prefix, highlight, suffix);//高亮处理
			SetCurPos(2, init_row + count++);//每次光标从init_row行开始，再加上count++；
			//打印文件名
			cout << prefix;
			ColourPrintf(highlight.c_str());
			cout << suffix;
			//打印路径
			SetCurPos(33, init_row + count - 1);
			printf("%-50s", doc_path.c_str());
		}
		SystemEnd();//设置光标到末尾--（针对的是控制台每次打印都有“请按任意键继续...”，将其设置到界面最末尾）
		system("pause");
	}

	system("pause");
	return 0;
}




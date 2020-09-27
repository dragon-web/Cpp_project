#include"Sysutil.h"
#include"sqlite3.h"
#include"DataManager.h"
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
int main()
{
	//Test_Sqlite();
	Test_Manager();
	system("pause");
	return 0;
}

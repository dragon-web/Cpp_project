#include"Sysutil.h"
#include"sqlite3.h"

//��̬����ʽ������Դ����ʽ���ã��ڳ�������ʹ�����ݿ�
 


void Test_Sqlite()
{
	sqlite3 *db;
	int rc = sqlite3_open("test.db", &db);
	if (rc != SQLITE_OK)
	{
		printf("Open databases failed\n");
	}
	else
	{
		printf("Open databases succese \n");
	}


	sqlite3_close(db);
}


int main()
{
	Test_Sqlite();

	system("pause");
	return 0;
}

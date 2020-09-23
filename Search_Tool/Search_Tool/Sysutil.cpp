#include"Sysutil.h"
#include"./sqlite3/sqlite3.h"

//静态库形式或者是源码形式调用，在程序里面使用数据库
 
#pragma comment(lib,"./sqlite/sqlite3.lib")

void DirectionList(const string& path, vector<string>& subfile, vector<string> & subdir)
{
	string _path = path;
	_path += "\\*.*";
	_finddata_t file;
	long handdle = _findfirst(_path.c_str(),&file);
	if (handdle == -1)
	{
		perror("_findfirst");
		return;
	}
	do
	{
		if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
		{
			continue;
		}
		if (file.attrib & _A_SUBDIR)
		{
			/*cout << file.name << endl;
			string temp_path = path;
			temp_path += "\\";
			temp_path += file.name;
			DirectionList(temp_path);*/
			subdir.push_back(file.name);
		}
		else
			//cout << file.name << endl;
			subfile.push_back(file.name);
	} while (_findnext(handdle,&file) == 0);

	_findclose(handdle);
}

void Test_DirectionList()
{
	const string& path = "C:\\Users\\飞龙\\Desktop\\BIT C++初阶课件-2019-V4";
	vector<string> subfile, subdir;
	DirectionList(path,subfile,subdir);
	for (const auto &e : subfile)
	{
		cout << e << endl;
	}
	for (const auto &e : subdir)
	{
		cout << e << endl;
	}
}

int main()
{
	Test_DirectionList();
	system("pause");
	return 0;
}
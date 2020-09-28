#include"ScanManager.h"
#include"Sysutil.h"

//set 底层会对关键码进行排序  将底层数据加入到集合里面，这样就会对集合进行排序
/*
void ScanManager::ScanDirectory(const string &path)
{
	//需要扫描本地文件系统
	//扫描数据库文件系统 
	//实现二者的同步（增删查改）
	vector<string> local_files;  //sub_files
	vector<string> local_dirs;   //sub_dires
	DirectionList(path, local_files, local_dirs);
	set<string> local_set;
	local_set.insert(local_files.begin(), local_files.end());
	local_set.insert(local_dirs.begin(), local_dirs.end());

	set<string> db_set;
	//需要从数据库拿到所有数据


}
*/
#include"ScanManager.h"
#include"Sysutil.h"
#include"DataManager.h"

//set 底层会对  关键码  进行排序  将底层数据加入到集合里面，这样就会对集合进行排序
ScanManager::ScanManager()
{}

ScanManager& ScanManager::CreateInstance(const string &path)
{
	static ScanManager inst;
	thread scan_thread(&StartScan,&inst,path);
	scan_thread.detach();
	return inst;
}

void ScanManager::StartScan(const string &path)
{
	while (1)
	{
		this_thread::sleep_for(chrono::seconds(3)); //每隔3秒扫描一次
		ScanDirectory(path);
	}
}

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

	DataManager &m_db = DataManager::GetInstance();

	multiset<string> db_set;
	//需要从数据库拿到所有数据
	m_db.GetDocs(path,db_set);

	auto local_it = local_set.begin();
	auto db_it = db_set.begin();
	while (local_it != local_set.end() && db_it != db_set.end())
	{
		if (*local_it < *db_it)
		{
			//本地文件存在，数据库文件不存在,数据库新增文件
			m_db.InsertDoc(path, *local_it);
			local_it++;

		}
		else if(*local_it > *db_it)
		{
			//本地文件不存在，数据库文件存在，数据库删除文件
			m_db.DeleteDoc(path, *db_it);
			db_it++;
		}
		else
		{
			//本地和数据库文件都存在，数据库文件不需要改变
			local_it++;
			db_it++;
		}
	}
	while (local_it != local_set.end())
	{
		//本地文件存在，数据库文件不存在，数据库插入新文件
		m_db.InsertDoc(path, *local_it);
		local_it++;
	}
	while (db_it != db_set.end())
	{
		//数据库多有文件，将数据库多出部分进行删除
		m_db.DeleteDoc(path, *db_it);
		db_it++;
	}
	//递归遍历子目录
	for (const auto &dir : local_dirs)
	{
		string dir_path = path;  //一开始目录
		dir_path += "\\";  
		dir_path += dir;
		ScanDirectory(dir_path);
	}
}



#include"ScanManager.h"
#include"Sysutil.h"
#include"DataManager.h"

//set �ײ���  �ؼ���  ��������  ���ײ����ݼ��뵽�������棬�����ͻ�Լ��Ͻ�������
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
		this_thread::sleep_for(chrono::seconds(3)); //ÿ��3��ɨ��һ��
		ScanDirectory(path);
	}
}

void ScanManager::ScanDirectory(const string &path)
{
	//��Ҫɨ�豾���ļ�ϵͳ
	//ɨ�����ݿ��ļ�ϵͳ 
	//ʵ�ֶ��ߵ�ͬ������ɾ��ģ�
	vector<string> local_files;  //sub_files
	vector<string> local_dirs;   //sub_dires
	DirectionList(path, local_files, local_dirs);
	set<string> local_set;
	local_set.insert(local_files.begin(), local_files.end());
	local_set.insert(local_dirs.begin(), local_dirs.end());  

	DataManager &m_db = DataManager::GetInstance();

	multiset<string> db_set;
	//��Ҫ�����ݿ��õ���������
	m_db.GetDocs(path,db_set);

	auto local_it = local_set.begin();
	auto db_it = db_set.begin();
	while (local_it != local_set.end() && db_it != db_set.end())
	{
		if (*local_it < *db_it)
		{
			//�����ļ����ڣ����ݿ��ļ�������,���ݿ������ļ�
			m_db.InsertDoc(path, *local_it);
			local_it++;

		}
		else if(*local_it > *db_it)
		{
			//�����ļ������ڣ����ݿ��ļ����ڣ����ݿ�ɾ���ļ�
			m_db.DeleteDoc(path, *db_it);
			db_it++;
		}
		else
		{
			//���غ����ݿ��ļ������ڣ����ݿ��ļ�����Ҫ�ı�
			local_it++;
			db_it++;
		}
	}
	while (local_it != local_set.end())
	{
		//�����ļ����ڣ����ݿ��ļ������ڣ����ݿ�������ļ�
		m_db.InsertDoc(path, *local_it);
		local_it++;
	}
	while (db_it != db_set.end())
	{
		//���ݿ�����ļ��������ݿ������ֽ���ɾ��
		m_db.DeleteDoc(path, *db_it);
		db_it++;
	}
	//�ݹ������Ŀ¼
	for (const auto &dir : local_dirs)
	{
		string dir_path = path;  //һ��ʼĿ¼
		dir_path += "\\";  
		dir_path += dir;
		ScanDirectory(dir_path);
	}
}



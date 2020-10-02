#include"ScanManager.h"
#include"Sysutil.h"

//set �ײ���  �ؼ���  ��������  ���ײ����ݼ��뵽�������棬�����ͻ�Լ��Ͻ�������

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

	set<string> db_set;
	//��Ҫ�����ݿ��õ���������
	m_db.GetDocs(path,db_set);

	auto local_it = local_set.begin();
	auto db_it = db_set.begin();
	while (local_it != local_set.end() && db_it != db_set.end())
	{
		if (*local_it < *db_it)
		{
			//�����ļ����ڣ����ݿ��ļ�������,���ݿ������ļ�
			

		}
		else if(*local_it > *db_it)
		{
			//�����ļ������ڣ����ݿ��ļ����ڣ����ݿ�ɾ���ļ�

		}
		else
		{
			//���غ����ݿ��ļ������ڣ����ݿ��ļ�����Ҫ�ı�
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
		}
		//�ݹ������Ŀ¼

	}


}

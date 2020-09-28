#pragma once

#include"common.h"
#include"DataManager.h"


class ScanManager
{
public:
	void ScanDirectory(const string &path);
private:
	DataManager m_db;
};
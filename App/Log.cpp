#include "stdafx.h"


vector<string> Log::m_Log;

string logString[] = {
	"ERROR: ",
	"WARNING: ",
	"DEBUG: " };

Log::Log()
{
}


Log::~Log()
{
}

void Log::Message(LogType type, string infomarion)
{
	
	string t = logString[type] + infomarion;
	m_Log.push_back(t);
	if (type == LOG_ERROR) system("color 4");
	else if(type ==LOG_WARNING) system("color 1");
	else system("color 7");
	cout << t << endl;
}

void Log::OutputFile()
{
	std::ofstream f("Log.txt");

	for (size_t i = 0; i < m_Log.size(); i++)
	{
		f << m_Log[i] << endl;
	}

	f.close();
}

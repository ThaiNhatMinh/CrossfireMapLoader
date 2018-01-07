#pragma once

class Log
{
private:
	static vector<string> m_Log;
public:
	enum LogType
	{
		LOG_ERROR,
		LOG_WARNING,
		LOG_DEBUG,
		LOG_NUM,
	};
	Log();
	~Log();

	static void Message(LogType type,string infomarion);
	static void OutputFile();
};

#define E_ERROR(x) Log::Message(Log::LOG_ERROR,(x))
#define E_WARNING(x) Log::Message(Log::LOG_WARNING,(x))
//#define E_DEBUG(x) Log::Message(Log::LOG_DEBUG,(x))
#define E_ASSERT(expr) 
#define E_DEBUG(x)
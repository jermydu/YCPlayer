#include "YCThread.h"

YCThread::YCThread() 
{

}
YCThread::~YCThread()
{

}

void YCThread::Start()
{
	if (!m_spThread)
		m_spThread.reset(new std::thread(std::bind(&YCThread::ThreadProc, this)));
}

void YCThread::ThreadProc()
{
	Run();
}

void YCThread::Join()
{
	if (m_spThread && m_spThread->joinable())
		m_spThread->join();
}





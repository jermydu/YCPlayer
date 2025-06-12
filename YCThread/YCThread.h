#ifndef __YCTHREAD_
#define __YCTHREAD_

#include <thread>
#include <memory>
#include <functional>
#include <memory>

class YCThread
{
public:
	YCThread();
	virtual ~YCThread();

	YCThread(const YCThread&) = delete;
	YCThread& operator=(const YCThread&) = delete;

	//开始线程
	void Start();
	//结束线程
	virtual void Stop() = 0;
	//等待线程退出
	void Join();

protected:
	virtual void Run() = 0;
	bool     m_bStop{ false };

private:
	//线程入口函数
	void ThreadProc();
	std::shared_ptr<std::thread> m_spThread{ nullptr };
};

#endif // !__YCTHREAD_





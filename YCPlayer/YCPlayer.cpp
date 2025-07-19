// YCPlayer.cpp: 定义应用程序的入口点。
//

#include "YCPlayer.h"
#include "YCCommonDef.h"
#include "YCAV.h"
#include "YCLogger.h"
#include <memory>
using namespace std;
using namespace YCAV;

int main()
{
	// 初始化日志系统，指定日志文件路径
	YCLogger::Logger::Initialize("app.log", false);

	// 设置日志级别为 debug
	YCLogger::Logger::SetLogLevel(YCLogger::LoggerLevel::Debug);

	YCAVReader reader;
	const char videoPath[]  = "./testvideo3.mp4";
	YCRet ret = reader.Open(videoPath);
	if (ret != YCRet::YCRet_OK)
	{
		LOG_ERROR("open file {0} faild!!!", { videoPath });
	}

	while (1)
	{
		YCAVPacket readYCAVPacket;
		ret = reader.Read(&readYCAVPacket);
		if (ret == YCRet::YCRet_EOF)
		{
			LOG_INFO("read frame eof");
			break;
		}
		else if (ret != YCRet::YCRet_OK)
		{
			LOG_ERROR("read frame error!!!");
		}
		else
		{
			LOG_INFO("read frame success");
		}
	}

	reader.Close();

	// 关闭日志系统
	YCLogger::Logger::Shutdown();

	return 0;
}

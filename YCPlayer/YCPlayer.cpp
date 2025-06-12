// YCPlayer.cpp: 定义应用程序的入口点。
//

#include "YCPlayer.h"
#include "spdlog/spdlog.h"
#include "YCCommonDef.h"
#include "YCAV.h"

using namespace std;
using namespace YCAV;

int main()
{
	YCAVReader reader;
	const char videoPath[]  = "./testvideo3.mp4";
	YCRet ret = reader.Open(videoPath);
	if (ret != YCRet::YCRet_OK)
	{
		spdlog::error("open file {} faild!!!", videoPath);
	}

	while (1)
	{
		YCAVPacket* pYCAVPacket = new YCAVPacket();
		ret = reader.Read(pYCAVPacket);
		if (ret == YCRet::YCRet_EOF)
		{
			spdlog::info("read frame eof");
			break;
		}
		else if (ret != YCRet::YCRet_OK)
		{
			spdlog::error("read frame error!!!");
		}
		else
		{
			spdlog::info("read frame success");
		}
	}

	reader.Close();

	return 0;
}

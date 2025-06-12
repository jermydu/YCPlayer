#ifndef _YCAVCLASSPRIVATE_H
#define _YCAVCLASSPRIVATE_H

extern "C"
{
#include "libavformat/avformat.h"
}

#include "YCCommonDef.h"
#include "spdlog/spdlog.h"
#include <string>
using namespace std;

namespace YCAV
{
	class YCAVPacketPrivate
	{
	public:
		AVPacket* pAvPacket{ nullptr };
	};

	class YCAVFormatContextPrivate
	{
	public:
		AVFormatContext* pAvFormatContext = nullptr;
	};

	string GetFfmpegErrorString(int err);
}



#endif  //_YCAVCLASSPRIVATE_H
#ifndef _YCAVCLASSPRIVATE_H
#define _YCAVCLASSPRIVATE_H

extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/frame.h"
#include "libavutil/pixdesc.h"
}

#include "YCCommonDef.h"
#include "YCLogger.h"
#include <string>
using namespace std;

namespace YCAV
{
	class YCAVPacketPrivate
	{
	public:
		AVPacket* pAvPacket{ nullptr };
	};

	class YCAVFormatPrivate
	{
	public:
		AVFormatContext* pAvFormatContext{ nullptr };
	};

	class YCAVDecoderPrivate
	{
	public:
		AVCodecContext* pAvCodecContext{ nullptr };
	};

	class YCAVStreamPrivate
	{
	public:
		AVCodecParameters* pAvCodecParameters{ nullptr }; //流的编解码参数
	};

	class YCAVFramePrivate
	{
	public:
		AVFrame* pAvFrame{ nullptr }; //解码后的视频帧	
	};

	string GetFfmpegErrorString(int err);
}



#endif  //_YCAVCLASSPRIVATE_H
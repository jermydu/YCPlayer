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
#include "YCAV.h"
using namespace std;

namespace YCLIB {
	class YCAVPacketPrivate {
	public:
		AVPacket* pAvPacket{ nullptr };
		YCAVPacketPrivate();
		virtual ~YCAVPacketPrivate();
	};

	struct YCAVPacket::Imp {
		std::unique_ptr<YCAVPacketPrivate> pPacketPrivate;
		Imp() :pPacketPrivate(std::make_unique<YCAVPacketPrivate>()) {

		}
	};

	class YCAVFormatPrivate {
	public:
		AVFormatContext* pAvFormatContext{ nullptr };
		YCAVFormatPrivate();
		virtual ~YCAVFormatPrivate();
	};

	struct YCAVReader::Imp {
		std::unique_ptr<YCAVFormatPrivate> pFormatPrivate;
		Imp() :pFormatPrivate(std::make_unique<YCAVFormatPrivate>()) {

		}
	};

	class YCAVDecoderPrivate {
	public:
		AVCodecContext* pAvCodecContext{ nullptr };
	};

	class YCAVStreamPrivate {
	public:
		AVCodecParameters* pAvCodecParameters{ nullptr }; //流的编解码参数
	};

	class YCAVFramePrivate {
	public:
		AVFrame* pAvFrame{ nullptr }; //解码后的视频帧	
	};

	string GetFfmpegErrorString(int err);
}



#endif  //_YCAVCLASSPRIVATE_H
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
	struct YCAVPacketPrivate {
		AVPacket* pAvPacket{ nullptr };
		YCAVPacketPrivate();
		virtual ~YCAVPacketPrivate();
	};

	struct YCAVPacket::Imp {
		std::unique_ptr<YCAVPacketPrivate> pPacketPrivate;
		Imp() :pPacketPrivate(std::make_unique<YCAVPacketPrivate>()) {

		}
	};

	struct YCAVFormatPrivate {
		AVFormatContext* pAvFormatContext{ nullptr };
		YCAVFormatPrivate();
		virtual ~YCAVFormatPrivate();
	};

	struct YCAVReader::Imp {
		std::unique_ptr<YCAVFormatPrivate> pFormatPrivate;
		Imp() :pFormatPrivate(std::make_unique<YCAVFormatPrivate>()) {

		}
	};

	struct YCAVDecoderPrivate {
		AVCodecContext* pAvCodecContext{ nullptr };
		YCAVDecoderPrivate();
		virtual ~YCAVDecoderPrivate();
	};

	struct YCAVDecoder::Imp {
		std::unique_ptr<YCAVDecoderPrivate> pDecoderPrivate;
		Imp() :pDecoderPrivate(std::make_unique<YCAVDecoderPrivate>()) {

		}
	};

	struct YCAVStreamPrivate {
		AVCodecParameters* pAvCodecParameters{ nullptr }; //流的编解码参数
		YCAVStreamPrivate();
		virtual ~YCAVStreamPrivate();
	};

	struct YCAVStream::Imp {
		std::unique_ptr<YCAVStreamPrivate> pStreamPrivate;
		Imp() :pStreamPrivate(std::make_unique<YCAVStreamPrivate>()) {

		}
	};

	struct YCAVFramePrivate {
		AVFrame* pAvFrame{ nullptr }; //解码后的视频帧	
		YCAVFramePrivate();
		virtual ~YCAVFramePrivate();
	};

	struct YCAVFrame::Imp {
		std::unique_ptr<YCAVFramePrivate> pFramePrivate;
		Imp() : pFramePrivate(std::make_unique<YCAVFramePrivate>()) {

		}
	};

	string GetFfmpegErrorString(int err);
}



#endif  //_YCAVCLASSPRIVATE_H
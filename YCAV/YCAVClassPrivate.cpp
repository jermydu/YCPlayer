#include "YCAVClassPrivate.h"

namespace YCLIB {
	YCAVPacketPrivate::YCAVPacketPrivate() {
		pAvPacket = av_packet_alloc();
	}
	YCAVPacketPrivate::~YCAVPacketPrivate() {
		if (pAvPacket) {
			av_packet_free(&pAvPacket);
			pAvPacket = nullptr;
		}
	}

	YCAVFormatPrivate::YCAVFormatPrivate() {
		pAvFormatContext = avformat_alloc_context();
	}
	YCAVFormatPrivate::~YCAVFormatPrivate() {
		if (pAvFormatContext) {
			avformat_free_context(pAvFormatContext);
			pAvFormatContext = nullptr;
		}
	}

	YCAVFramePrivate::YCAVFramePrivate() {
		pAvFrame = av_frame_alloc();
	}
	YCAVFramePrivate::~YCAVFramePrivate() {
		if (pAvFrame) {
			av_frame_free(&pAvFrame);
			pAvFrame = nullptr;
		}
	}

	YCAVStreamPrivate::YCAVStreamPrivate() {
		pAvCodecParameters = avcodec_parameters_alloc();
	}
	YCAVStreamPrivate::~YCAVStreamPrivate() {
		if (pAvCodecParameters) {
			avcodec_parameters_free(&pAvCodecParameters);
			pAvCodecParameters = nullptr;
		}
	}

	YCAVDecoderPrivate::YCAVDecoderPrivate() {
		pAvCodecContext = avcodec_alloc_context3(nullptr);
	}
	YCAVDecoderPrivate::~YCAVDecoderPrivate() {
		if (pAvCodecContext) {
			avcodec_free_context(&pAvCodecContext);
			pAvCodecContext = nullptr;
		}
	}
}
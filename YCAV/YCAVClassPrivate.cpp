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
}
#include "YCAV.h"
#include "YCAVClassPrivate.h"

namespace YCLIB {
	YCAVPacket::YCAVPacket() :imp(new Imp) {
	}

	YCAVPacket::~YCAVPacket() {
		if (imp) {
			delete imp;
			imp = nullptr;
		}
	}

	int YCAVPacket::GetStreamIndex() const {
		return imp->pPacketPrivate->pAvPacket->stream_index;
	}
}
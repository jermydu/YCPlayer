#include "YCAV.h"
#include "YCAVClassPrivate.h"

namespace YCLIB 
{
	YCAVPacket::YCAVPacket()
	{
		imp = new YCAVPacketPrivate();
		imp->pAvPacket = av_packet_alloc();
	}

	YCAVPacket::~YCAVPacket()
	{
		if (imp->pAvPacket)
		{
			av_packet_free(&imp->pAvPacket);
			imp->pAvPacket = nullptr;
		}
		if (imp)
		{
			delete imp;
			imp = nullptr;
		}
	}

	int YCAVPacket::GetStreamIndex() const
	{
		return imp->pAvPacket->stream_index;
	}
}
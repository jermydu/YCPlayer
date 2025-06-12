#include "YCAV.h"
#include "YCAVClassPrivate.h"

namespace YCAV 
{
	YCAVPacket::YCAVPacket()
	{
		pAvPacketImp = new YCAVPacketPrivate();
		pAvPacketImp->pAvPacket = av_packet_alloc();
	}

	YCAVPacket::~YCAVPacket()
	{
		if (pAvPacketImp->pAvPacket)
		{
			av_packet_free(&pAvPacketImp->pAvPacket);
			pAvPacketImp->pAvPacket = nullptr;
		}
		if (pAvPacketImp)
		{
			delete pAvPacketImp;
			pAvPacketImp = nullptr;
		}
	}
}
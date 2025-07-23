#include "YCAV.h"
#include "YCAVClassPrivate.h"

namespace YCLIB
{
	YCAVStream::YCAVStream()
	{
		imp = new YCAVStreamPrivate();
		imp->pAvCodecParameters = avcodec_parameters_alloc();
	}

	YCAVStream::~YCAVStream()
	{
		if (imp->pAvCodecParameters)
		{
			avcodec_parameters_free(&imp->pAvCodecParameters);
			imp->pAvCodecParameters = nullptr;
		}
		if (imp)
		{
			delete imp;
			imp = nullptr;
		}
	}
}

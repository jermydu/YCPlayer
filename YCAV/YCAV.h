#ifndef _YCAV_H
#define _YCAV_H

#include "YCCommonDef.h"

namespace YCAV {

	class YCAVPacket {
	public:

	};
	
	class YCAVReader {
	public:
		YCAVReader();
		virtual ~YCAVReader();

		YCRet Open(const char* path);
		YCRet Close();

		YCRet Read(YCAVPacket* packet);
		
	private:
		AVFormatContext* pAvFormatContext = nullptr;
	};
}

#endif //_YCAV_H
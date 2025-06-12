#include "YCAVClassPrivate.h"

namespace YCAV
{
	string GetFfmpegErrorString(int err)
	{
		char tmp[AV_ERROR_MAX_STRING_SIZE] = { 0 };
		av_strerror(err, tmp, AV_ERROR_MAX_STRING_SIZE);
		string ret = "";
		ret.append(tmp, AV_ERROR_MAX_STRING_SIZE);
		return ret;
	}
}
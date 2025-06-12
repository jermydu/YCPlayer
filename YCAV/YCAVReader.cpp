#include "YCAV.h"
#include "YCAVClassPrivate.h"

namespace YCAV 
{
	YCAVReader::YCAVReader() 
	{
		pAVFormatContextImp = new YCAVFormatContextPrivate();
		pAVFormatContextImp->pAvFormatContext = avformat_alloc_context();
	}

	YCAVReader::~YCAVReader() 
	{
		if (pAVFormatContextImp->pAvFormatContext)
		{
			avformat_free_context(pAVFormatContextImp->pAvFormatContext);
			pAVFormatContextImp->pAvFormatContext = nullptr;
		}
		if (pAVFormatContextImp)
		{
			delete pAVFormatContextImp;
			pAVFormatContextImp = nullptr;
		}
	}

	YCRet YCAVReader::Open(const char* path) 
	{
		if (pAVFormatContextImp->pAvFormatContext == nullptr)
		{
			return YCRet::YCRet_CreateContextFailed;
		}
		if (path == nullptr || strcmp(path, "") == 0) 
		{
			return YCRet::YCRet_NotExist;
		}

		int ret = avformat_open_input(&pAVFormatContextImp->pAvFormatContext, path, nullptr, nullptr);
		if (ret != 0)
		{
			spdlog::error("avformat_open_input failed: error={}", GetFfmpegErrorString(ret));
			return YCRet::YCRet_OpenFileFailed;
		}
		ret = avformat_find_stream_info(pAVFormatContextImp->pAvFormatContext, nullptr);
		if (ret != 0) 
		{
			spdlog::error("avformat_find_stream_info failed: error={}", GetFfmpegErrorString(ret));
			return YCRet::YCRet_OpenFileFailed;
		}
		else 
		{
			spdlog::debug("############################################################");
			//打印输入输出格式详细信息
			spdlog::debug("=================Print detailed information about {0}", path);
			av_dump_format(pAVFormatContextImp->pAvFormatContext, 0, path, 0);
			spdlog::debug("############################################################");
		}
		return YCRet::YCRet_OK;
	}

	YCRet YCAVReader::Close() 
	{
		if (pAVFormatContextImp->pAvFormatContext == nullptr)
		{
			return YCRet::YCRet_CreateContextFailed;
		}
		avformat_close_input(&pAVFormatContextImp->pAvFormatContext);
		return YCRet::YCRet_OK;
	}

	YCRet YCAVReader::Read(YCAVPacket* pPacket)
	{
		if (pAVFormatContextImp->pAvFormatContext == nullptr)
		{
			return YCRet::YCRet_CreateContextFailed;
		}
		int ret = av_read_frame(pAVFormatContextImp->pAvFormatContext, pPacket->pAvPacketImp->pAvPacket);
		if (ret == 0)
		{
			return YCRet::YCRet_OK;
		}
		else 
		{
			if (ret == AVERROR_EOF) 
			{
				return YCRet::YCRet_EOF;
			}
			else if (ret == AVERROR_EXIT) 
			{
				return YCRet::YCRet_ReadPacketTimeOut;
			}
			else if (ret == AVERROR(EIO)) 
			{
				return YCRet::YCRet_ReadPacketIOError;
			}
			else 
			{
				return YCRet::YCRet_ReadPacketOtherError;
			}
		}
	}
}
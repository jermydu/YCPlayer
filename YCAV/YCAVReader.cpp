#include "YCAV.h"

namespace YCAV {
	YCAVReader::YCAVReader() {
		pAvFormatContext = avformat_alloc_context();

	}

	YCAVReader::~YCAVReader() {
		if (pAvFormatContext) {
			avformat_free_context(pAvFormatContext);
			pAvFormatContext = nullptr;
		}
	}

	YCRet YCAVReader::Open(const char* path) {
		if (pAvFormatContext == nullptr) {
			return YCRet::YCRet_CreateContextFailed;
		}
		if (path == nullptr || strcmp(path, "") == 0) {
			return YCRet::YCRet_NotExist;
		}

		int ret = avformat_open_input(&pAvFormatContext, path, nullptr, nullptr);
		if (ret != 0) {
			spdlog::error("avformat_find_stream_info failed: error={}", GetFfmpegErrorString(ret));
			return YCRet::YCRet_OpenFileFailed;
		}
		ret = avformat_find_stream_info(pAvFormatContext, nullptr);
		if (ret != 0) {
			spdlog::error("avformat_find_stream_info failed: error={}", GetFfmpegErrorString(ret));
			return YCRet::YCRet_OpenFileFailed;
		}
		else {
			spdlog::debug("############################################################");
		}
		return YCRet::YCRet_OK;
	}

	YCRet YCAVReader::Close() {
		if (pAvFormatContext == nullptr) {
			return YCRet::YCRet_CreateContextFailed;
		}
		avformat_close_input(&pAvFormatContext);
		return YCRet::YCRet_OK;
	}

	YCRet YCAVReader::Read(YCAVPacket* packet) {
		return YCRet::YCRet_OK;
	}
}
#include "YCAV.h"
#include "YCAVClassPrivate.h"

namespace YCLIB {
	YCAVDecoder::YCAVDecoder() {
		imp = new YCAVDecoderPrivate();
		imp->pAvCodecContext = avcodec_alloc_context3(nullptr);
	}

	YCAVDecoder::~YCAVDecoder() {
		if (imp->pAvCodecContext) {
			avcodec_free_context(&imp->pAvCodecContext);
			imp->pAvCodecContext = nullptr;
		}
		if (imp) {
			delete imp;
			imp = nullptr;
		}
	}

	int YCAVDecoder::GetDecoderIndex() const {
		return decoderIndex;
	}

	YCRet YCAVDecoder::Init(YCAVStream* pYcAvStream) {
		decoderIndex = pYcAvStream->streamIndex;
		avcodec_parameters_to_context(imp->pAvCodecContext, pYcAvStream->imp->pAvCodecParameters);
		//查找解码器
		const AVCodec* pAvCodec = avcodec_find_decoder(imp->pAvCodecContext->codec_id);
		if (!pAvCodec) {
			LOG_ERROR("avcodec_find_decoder failed: codec_id={0}", { std::to_string(imp->pAvCodecContext->codec_id) });
			return YCRet::YCRet_FindDecoderFailed;
		}
		//打开解码器
		int ret = avcodec_open2(imp->pAvCodecContext, pAvCodec, nullptr);
		if (ret != 0) {
			LOG_ERROR("avcodec_open2 failed: error={0}", { GetFfmpegErrorString(ret) });
			return YCRet::YCRet_OpenDecoderFailed;
		}
		LOG_INFO("Decoder opened successfully: codec_id={0}", { std::to_string(imp->pAvCodecContext->codec_id) });


		return YCRet::YCRet_OK;
	}

	YCRet YCAVDecoder::Release() {
		if (imp->pAvCodecContext) {
			avcodec_close(imp->pAvCodecContext);

		}
		return YCRet::YCRet_OK;
	}

	YCRet YCAVDecoder::SendPacket(YCAVPacket* pYcAvPacket) {
		int ret = -1;
		if (pYcAvPacket == nullptr) {
			ret = avcodec_send_packet(imp->pAvCodecContext, nullptr);
		}
		else {
			ret = avcodec_send_packet(imp->pAvCodecContext, pYcAvPacket->imp->pPacketPrivate->pAvPacket);
		}
		if (ret != 0) {
			if (ret == AVERROR(EAGAIN)) {
				LOG_INFO("avcodec_send_packet need more data");
				return YCRet::YCRet_NeedMorePacket;
			}
			else if (ret == AVERROR_EOF) {
				LOG_INFO("avcodec_send_packet end of file");
				return YCRet::YCRet_EOF;
			}
			else if (ret < 0) {
				LOG_ERROR("avcodec_send_packet unknown error: error={0}", { GetFfmpegErrorString(ret) });
				return YCRet::YCRet_ReadPacketOtherError;
			}
			else {
				LOG_ERROR("avcodec_send_packet failed: error={0}", { GetFfmpegErrorString(ret) });
				return YCRet::YCRet_SendAvPacketFailed;
			}
		}

		return YCRet::YCRet_OK;
	}
	YCRet YCAVDecoder::ReceiveFrame(YCAVFrame* pYcAvFrame) {
		int ret = avcodec_receive_frame(imp->pAvCodecContext, pYcAvFrame->imp->pAvFrame);
		if (ret != 0) {
			if (ret == AVERROR(EAGAIN)) {
				//LOG_INFO("avcodec_receive_frame need more data");
				return YCRet::YCRet_NeedMorePacket;
			}
			else if (ret == AVERROR_EOF) {
				//LOG_INFO("avcodec_receive_frame end of file");
				return YCRet::YCRet_EOF;
			}
			else {
				//LOG_ERROR("avcodec_receive_frame unknown error: error={0}", { GetFfmpegErrorString(ret) });
				return YCRet::YCRet_ReceiveAvFrameFailed;
			}
		}
		return YCRet::YCRet_OK;
	}
}
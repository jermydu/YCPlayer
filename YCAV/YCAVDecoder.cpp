#include "YCAV.h"
#include "YCAVClassPrivate.h"

namespace YCLIB {
	YCAVDecoder::YCAVDecoder():imp(new Imp) {
	}

	YCAVDecoder::~YCAVDecoder() {
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
		avcodec_parameters_to_context(imp->pDecoderPrivate->pAvCodecContext, pYcAvStream->imp->pStreamPrivate->pAvCodecParameters);
		//查找解码器
		const AVCodec* pAvCodec = avcodec_find_decoder(imp->pDecoderPrivate->pAvCodecContext->codec_id);
		if (!pAvCodec) {
			LOG_ERROR("avcodec_find_decoder failed: codec_id={0}", { std::to_string(imp->pDecoderPrivate->pAvCodecContext->codec_id) });
			return YCRet::YCRet_FindDecoderFailed;
		}
		//打开解码器
		int ret = avcodec_open2(imp->pDecoderPrivate->pAvCodecContext, pAvCodec, nullptr);
		if (ret != 0) {
			LOG_ERROR("avcodec_open2 failed: error={0}", { GetFfmpegErrorString(ret) });
			return YCRet::YCRet_OpenDecoderFailed;
		}
		LOG_INFO("Decoder opened successfully: codec_id={0}", { std::to_string(imp->pDecoderPrivate->pAvCodecContext->codec_id) });


		return YCRet::YCRet_OK;
	}

	YCRet YCAVDecoder::Release() {
		if (imp->pDecoderPrivate->pAvCodecContext) {
			avcodec_close(imp->pDecoderPrivate->pAvCodecContext);

		}
		return YCRet::YCRet_OK;
	}

	YCRet YCAVDecoder::SendPacket(YCAVPacket* pYcAvPacket) {
		int ret = -1;
		if (pYcAvPacket == nullptr) {
			ret = avcodec_send_packet(imp->pDecoderPrivate->pAvCodecContext, nullptr);
		}
		else {
			ret = avcodec_send_packet(imp->pDecoderPrivate->pAvCodecContext, pYcAvPacket->imp->pPacketPrivate->pAvPacket);
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
		int ret = avcodec_receive_frame(imp->pDecoderPrivate->pAvCodecContext, pYcAvFrame->imp->pFramePrivate->pAvFrame);
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
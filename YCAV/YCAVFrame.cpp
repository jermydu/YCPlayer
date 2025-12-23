#include "YCAV.h"	
#include "YCAVClassPrivate.h"

namespace YCLIB {
	YCAVFrame::YCAVFrame() {
		imp = new YCAVFramePrivate();
		imp->pAvFrame = av_frame_alloc();
	}
	YCAVFrame::~YCAVFrame() {
		if (imp->pAvFrame) {
			av_frame_free(&imp->pAvFrame);
			imp->pAvFrame = nullptr;
		}
		if (imp) {
			delete imp;
			imp = nullptr;
		}
	}

	YCRet YCAVFrame::PrintVideoInfo() const {
		LOG_INFO("*********PrintVideoInfo:*********");
		LOG_INFO("Pixel Width: {0}", { std::to_string(imp->pAvFrame->width) });
		LOG_INFO("Pixel Height: {0}", { std::to_string(imp->pAvFrame->height) });

		AVPixelFormat format = static_cast<AVPixelFormat>(imp->pAvFrame->format);
		char* formatString = (char*)malloc(128);
		formatString = av_get_pix_fmt_string(formatString, 128, format);
		LOG_INFO("Pixel Format: {0}", { static_cast<string>(formatString) });

		free(formatString);

		for (int i = 0; i < AV_NUM_DATA_POINTERS; i++) {
			LOG_INFO("Linesize[{0}]:{1}", { std::to_string(i),std::to_string(imp->pAvFrame->linesize[i]) });
		}
		return YCRet::YCRet_OK;
	}

	YCRet YCAVFrame::PrintAudioInfo() const {
		LOG_INFO("*********PrintAudioInfo:*********");
		LOG_INFO("Audio Channel: {0}", { std::to_string(imp->pAvFrame->channels) });
		LOG_INFO("Audio nb_samples: {0}", { std::to_string(imp->pAvFrame->nb_samples) });
		LOG_INFO("Audio sample_rate: {0}", { std::to_string(imp->pAvFrame->sample_rate) });

		AVSampleFormat format = static_cast<AVSampleFormat>(imp->pAvFrame->format);
		char* formatString = (char*)malloc(128);
		formatString = av_get_sample_fmt_string(formatString, 128, format);
		LOG_INFO("Sample Format: {0}", { static_cast<string>(formatString) });

		free(formatString);

		for (int i = 0; i < AV_NUM_DATA_POINTERS; i++) {
			LOG_INFO("Linesize[{0}]:{1}", { std::to_string(i),std::to_string(imp->pAvFrame->linesize[i]) });
		}
		return YCRet::YCRet_OK;
	}

	int YCAVFrame::GetW() const {
		return imp->pAvFrame->width;
	}
	int YCAVFrame::GetH() const {
		return imp->pAvFrame->height;
	}
	void YCAVFrame::GetY(unsigned char* y) const {
		if (imp->pAvFrame->format == AV_PIX_FMT_YUV420P) {
			int width = GetW();
			int height = GetH();

			// YUV420P格式的Y分量存储在data[0]中
			// 每行的字节数由linesize[0]决定
			// 复制Y分量数据到y缓冲区
			// 注意：Y分量的宽度和高度与原始帧相同
			//linesize 存在字节对齐的问题 可能会导致每行的字节数linesize大于实际宽度width
			// menmcpy 拷贝实际的width字节数
			for (int i = 0; i < height; i++) {
				memcpy(y + i * width, imp->pAvFrame->data[0] + i * imp->pAvFrame->linesize[0], width);
			}
		}
		else {
			LOG_ERROR("Unsupported pixel format for Y channel extraction");
		}
	}
	void YCAVFrame::GetU(unsigned char* u) const {
		if (imp->pAvFrame->format == AV_PIX_FMT_YUV420P) {
			int width = GetW() / 2;
			int height = GetH() / 2;

			for (int i = 0; i < height; i++) {
				memcpy(u + i * width, imp->pAvFrame->data[1] + i * imp->pAvFrame->linesize[1], width);
			}
		}
		else {
			LOG_ERROR("Unsupported pixel format for U channel extraction");
		}
	}
	void YCAVFrame::GetV(unsigned char* v) const {
		if (imp->pAvFrame->format == AV_PIX_FMT_YUV420P) {
			int width = GetW() / 2;
			int height = GetH() / 2;

			for (int i = 0; i < height; i++) {
				memcpy(v + i * width, imp->pAvFrame->data[2] + i * imp->pAvFrame->linesize[2], width);
			}
		}
		else {
			LOG_ERROR("Unsupported pixel format for V channel extraction");
		}
	}

	int YCAVFrame::GetPcmSize() const {
		// 获取音频帧的样本数和每个样本的字节数
		int nb_samples = imp->pAvFrame->nb_samples; // 样本数
		AVSampleFormat format = static_cast<AVSampleFormat>(imp->pAvFrame->format);
		int bytes_per_sample = av_get_bytes_per_sample(format);

		if (bytes_per_sample < 0) {
			LOG_ERROR("Invalid sample format");
			return -1;
		}
		int channels = imp->pAvFrame->channels;
		return nb_samples * channels * bytes_per_sample; // 返回总的PCM数据大小
	}

	void YCAVFrame::GetPcm(unsigned char* pcm) const {
		int channels = imp->pAvFrame->channels;
		int nb_samples = imp->pAvFrame->nb_samples; //样本数
		AVSampleFormat format = static_cast<AVSampleFormat>(imp->pAvFrame->format);
		//获取每个样本的字节数
		int bytes_per_sample = av_get_bytes_per_sample(format);

		if (av_sample_fmt_is_planar(format)) {
			// planar: 每个通道分开存储
			/*for (int ch = 0; ch < channels; ++ch) {
				unsigned char* src = imp->pAvFrame->data[ch];
				for (int i = 0; i < nb_samples; ++i) {
					memcpy(pcm, src + i * bytes_per_sample, bytes_per_sample);
					pcm += bytes_per_sample;
				}
			}*/
			/*1.	fltp（float planar）格式说明
				•	FFmpeg 的 fltp 格式是每个通道的数据分开存储（左声道一块，右声道一块）。
				•	ffplay 播放原始 PCM 时，要求数据是 interleaved（交错）格式：L0,R0,L1,R1,...
			*/
			//使用ffplay播放时需要把planar格式转换为packed格式,否则播放的时候会有杂音且速度比较快 呵呵 
			for (int i = 0; i < nb_samples; ++i) {
				for (int ch = 0; ch < channels; ++ch) {
					unsigned char* src = imp->pAvFrame->data[ch];
					memcpy(pcm, src + i * bytes_per_sample, bytes_per_sample);
					pcm += bytes_per_sample;
				}
			}
		}
		else {
			// packed: 所有通道交错存储
			int frame_size = nb_samples * channels * bytes_per_sample;
			memcpy(pcm, imp->pAvFrame->data[0], frame_size);
		}
	}
}
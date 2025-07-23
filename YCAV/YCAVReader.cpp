#include "YCAV.h"
#include "YCAVClassPrivate.h"

namespace YCLIB 
{
	YCAVReader::YCAVReader() 
	{
		// 初始化日志系统，指定日志文件路径
		//YCLogger::Logger::Initialize("YCAV/YCAV.log", false);

		// 设置日志级别为 debug
		//YCLogger::Logger::SetLogLevel(YCLogger::LoggerLevel::Debug);

		imp = new YCAVFormatPrivate();
		imp->pAvFormatContext = avformat_alloc_context();
	}

	YCAVReader::~YCAVReader() 
	{
		if (imp->pAvFormatContext)
		{
			avformat_free_context(imp->pAvFormatContext);
			imp->pAvFormatContext = nullptr;
		}
		if (imp)
		{
			delete imp;
			imp = nullptr;
		}
		// 关闭日志系统
		//YCLogger::Logger::Shutdown();
	}

	YCRet YCAVReader::Open(const char* path) 
	{
		if (imp->pAvFormatContext == nullptr)
		{
			return YCRet::YCRet_CreateContextFailed;
		}
		if (path == nullptr || strcmp(path, "") == 0) 
		{
			return YCRet::YCRet_NotExist;
		}

		int ret = avformat_open_input(&imp->pAvFormatContext, path, nullptr, nullptr);
		if (ret != 0)
		{
			LOG_ERROR("avformat_open_input failed: error={0}", { GetFfmpegErrorString(ret) });
			return YCRet::YCRet_OpenFileFailed;
		}
		ret = avformat_find_stream_info(imp->pAvFormatContext, nullptr);
		if (ret != 0) 
		{
			LOG_ERROR("avformat_find_stream_info failed: error={0}", { GetFfmpegErrorString(ret) });
			return YCRet::YCRet_OpenFileFailed;
		}
		else 
		{
			LOG_INFO("############################################################");
			//打印输入输出格式详细信息

            //自己从avformatContex中取数据
            LOG_INFO("---avformat_find_stream_info");
            //路径名/文件名
            LOG_INFO("media name-->[{0}]", { imp->pAvFormatContext->url });
            //流媒体数量
            LOG_INFO("stream number-->[{0}]", { std::to_string(imp->pAvFormatContext->nb_streams) });
            //媒体文件码率 单位bps
            LOG_INFO("media bitrate-->[{0}kbps]", { std::to_string(imp->pAvFormatContext->bit_rate / 1024)});
            //duration  微妙->秒
            int totalSeconds = imp->pAvFormatContext->duration / AV_TIME_BASE;
            int hour = totalSeconds / 3600;
            int minute = (totalSeconds % 3600) / 60;
            int second = totalSeconds % 60;
            LOG_INFO("media duration-->[{0}:{1}:{2}]", { std::to_string(hour),std::to_string(minute),std::to_string(second), });
            
            LOG_INFO("---AVStream Info");
            //遍历方式读取视频信息和音频信息
			int nAudioIndex = -1;
			int nVideoIndex = -1;
            for (uint32_t i = 0; i < imp->pAvFormatContext->nb_streams; ++i)
            {
                //获取一个流
                AVStream* pAVStream = imp->pAvFormatContext->streams[i];
                //音频流
                if (AVMEDIA_TYPE_AUDIO == pAVStream->codecpar->codec_type)
                {
                    nAudioIndex = i;
                    LOG_INFO("---AVStream:Audio index:[{0}]", {std::to_string(pAVStream->index)});
                    //音频编解码器的采样率(每秒钟的采样数)  Hz
                    LOG_INFO("Audio samplerate->[{0}Hz]", { std::to_string(pAVStream->codecpar->sample_rate) });

                    //音频采样格式
                    if (AV_SAMPLE_FMT_FLTP == pAVStream->codecpar->format)
                    {
                        LOG_INFO("Audio sample format->[{0}]", { "AV_SAMPLE_FMT_FLTP"});
                    }
                    else if (AV_SAMPLE_FMT_S16P == pAVStream->codecpar->format)
                    {
                        LOG_INFO("Audio sample format->[{0}]", { "AV_SAMPLE_FMT_S16P" });
					}
					else if (AV_SAMPLE_FMT_S16 == pAVStream->codecpar->format)
					{
						LOG_INFO("Audio sample format->[{0}]", { "AV_SAMPLE_FMT_S16" });
					}
					else if (AV_SAMPLE_FMT_FLT == pAVStream->codecpar->format)
					{
						LOG_INFO("Audio sample format->[{0}]", { "AV_SAMPLE_FMT_FLT" });
					}
                    else
                    {   
                        LOG_INFO("Audio sample format->[{0}]", { std::to_string(pAVStream->codecpar->format)});
                    }
                    //音频通道数
                    LOG_INFO("Audio channel number->[{0}]", { std::to_string(pAVStream->codecpar->ch_layout.nb_channels) });
                    
                    //音频压缩编码格式
                    if (AV_CODEC_ID_AAC == pAVStream->codecpar->codec_id)
                    {
                        LOG_INFO("Audio codec->[{0}]", { "AAC"});
                    }
                    else if (AV_CODEC_ID_MP3 == pAVStream->codecpar->codec_id)
                    {
                        LOG_INFO("Audio codec->[{0}]", { "MP3"});
                    }
                    else
                    {
                        LOG_INFO("Audio codec->[{0}]", { std::to_string(pAVStream->codecpar->codec_id)});
                    }
                    //一帧单通道样本数
                    LOG_INFO("Audio frame size->[{0}]", { std::to_string(pAVStream->codecpar->frame_size) });
                    //帧率 fps = sample_rate / frame_size
                    //音频总时长 秒
                    if (pAVStream->duration != AV_NOPTS_VALUE)
                    {
                        //ffmpeg中的内部计时单位（时间基），ffmepg中的所有时间都是于它为一个单位，
                        //比如AVStream中的duration，即这个流的长度为duration个AV_TIME_BASE.
                        int durationAudio = (pAVStream->duration) * av_q2d(pAVStream->time_base);
                        LOG_INFO("Audio duration->[{0}:{1}:{2}]", { std::to_string(durationAudio / 3600),std::to_string((durationAudio % 3600) / 60),std::to_string(durationAudio % 60) });
                    }
                    else
                    {
                        LOG_INFO("Audio duration unknown");
                    }
                }
                //视频流
                else if (AVMEDIA_TYPE_VIDEO == pAVStream->codecpar->codec_type)
                {
                    nVideoIndex = i;
                    LOG_INFO("---AVStream:Video index:[{0}]",{std::to_string(pAVStream->index)});
                    //视频帧率 fps 表示每秒出现多少帧
                    LOG_INFO("Video fps->[{0}fps]", { std::to_string(av_q2d(pAVStream->avg_frame_rate)) });

                    //视频压缩编码格式
                    if (AV_CODEC_ID_MPEG4 == pAVStream->codecpar->codec_id)
                    {
                        LOG_INFO("Video codec->[{0}]", {"MPEG4"});
                    }
                    else if (AV_CODEC_ID_H264 == pAVStream->codecpar->codec_id)
                    {
                        LOG_INFO("Video codec->[{0}]", { "H264" });

					}
					else if (AV_CODEC_ID_H265 == pAVStream->codecpar->codec_id)
					{
						LOG_INFO("Video codec->[{0}]", { "H265" });
					}
					else if (AV_CODEC_ID_VP9 == pAVStream->codecpar->codec_id)
					{
						LOG_INFO("Video codec->[{0}]", { "VP9" });
					}
					else if (AV_CODEC_ID_AV1 == pAVStream->codecpar->codec_id)
					{
						LOG_INFO("Video codec->[{0}]", { "AV1" });
					}
                    else
                    {
                        LOG_INFO("Video codec->[{0}]", {std::to_string(pAVStream->codecpar->codec_id)});
                    }

                    //视频帧宽高
                    LOG_INFO("Video width->[{0}] height->[{1}]", { std::to_string(pAVStream->codecpar->width) ,std::to_string(pAVStream->codecpar->height) });

                    //视频总时长 秒
                    if (pAVStream->duration != AV_NOPTS_VALUE)
                    {
                        //ffmpeg中的内部计时单位（时间基），ffmepg中的所有时间都是于它为一个单位，
                        //比如AVStream中的duration，即这个流的长度为duration个AV_TIME_BASE.
                        int durationVideo = (pAVStream->duration) * av_q2d(pAVStream->time_base);
                        LOG_INFO("Video duration->[{0}:{1}:{2}]", { std::to_string(durationVideo / 3600) ,std::to_string((durationVideo % 3600) / 60),std::to_string(durationVideo % 60)});
                    }
                    else
                    {
                        LOG_INFO("Audio duration unknown");
                    }
                }
                //字幕流
                else if (AVMEDIA_TYPE_SUBTITLE == pAVStream->codecpar->codec_type)
                {
                    LOG_INFO("---AVStream:SubTitle index:[{0}]",{std::to_string(pAVStream->index)});
                }
            }
			LOG_INFO("############################################################");
		}
		return YCRet::YCRet_OK;
	}

	int YCAVReader::GetStreamCount() const
	{
		return imp->pAvFormatContext ? imp->pAvFormatContext->nb_streams : 0;
	}
	YCRet YCAVReader::GetStream(int index, YCAVStream* pYcAvStream) const
	{
		if (imp->pAvFormatContext)
		{
			AVStream* pAVStream = imp->pAvFormatContext->streams[index];
			pYcAvStream->streamIndex = pAVStream->index;
			avcodec_parameters_copy(pYcAvStream->imp->pAvCodecParameters,pAVStream->codecpar);

			return YCRet::YCRet_OK;
		}
		else
		{
			return YCRet::YCRet_CreateContextFailed;
		}
	}

	YCRet YCAVReader::Close() 
	{
		if (imp->pAvFormatContext == nullptr)
		{
			return YCRet::YCRet_CreateContextFailed;
		}
		avformat_close_input(&imp->pAvFormatContext);
		return YCRet::YCRet_OK;
	}

	YCRet YCAVReader::Read(YCAVPacket* pPacket)
	{
		if (imp->pAvFormatContext == nullptr)
		{
			return YCRet::YCRet_CreateContextFailed;
		}
		int ret = av_read_frame(imp->pAvFormatContext, pPacket->imp->pAvPacket);
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

	int YCAVReader::GetVideoStreamIndex()const
	{
		return av_find_best_stream(imp->pAvFormatContext, AVMediaType::AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
	}
	int YCAVReader::GetAudioStreamIndex()const 
	{
		return av_find_best_stream(imp->pAvFormatContext, AVMediaType::AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);

	}

}
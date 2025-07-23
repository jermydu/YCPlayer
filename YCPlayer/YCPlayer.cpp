// YCPlayer.cpp: 定义应用程序的入口点。
//

#include "YCPlayer.h"
#include "YCCommonDef.h"
#include "YCAV.h"
#include "YCLogger.h"
#include "YCQueue.h"
#include <memory>
#include <vector>
using namespace std;
using namespace YCLIB;

int main()
{
	// 初始化日志系统，指定日志文件路径
	Logger::instance().Initialize("YCPlayer.log", false);

	// 设置日志级别为 debug
	Logger::instance().SetLogLevel(LoggerLevel::Debug);

	YCAVReader reader;
	const char videoPath[]  = "./testvideo3.mp4";
	YCRet ret = reader.Open(videoPath);
	if (ret != YCRet::YCRet_OK)
	{
		LOG_ERROR("open file {0} faild!!!", { videoPath });
	}

	int iVideoStreamIndex = reader.GetVideoStreamIndex();
	int iAudioStreamIndex = reader.GetAudioStreamIndex();

	LOG_INFO("video stream index:{0}", { std::to_string(iVideoStreamIndex) });
	LOG_INFO("audio stream index:{0}", { std::to_string(iAudioStreamIndex) });

	vector<std::unique_ptr<YCAVDecoder>> vYcAvDecodersList;

	int iStreamCount = reader.GetStreamCount();
	for (int index = 0; index < iStreamCount; index++)
	{
		YCAVStream ycAvStream;
		YCRet ret = reader.GetStream(index,&ycAvStream);
		if (ret == YCRet::YCRet_OK)
		{
			LOG_INFO("streamIndex:{0}", { std::to_string(ycAvStream.streamIndex) });
			
		}
		std::unique_ptr<YCAVDecoder> pYcAvDecoder = std::make_unique<YCAVDecoder>();
		ret = pYcAvDecoder->Init(&ycAvStream);
		if (ret != YCRet::YCRet_OK)
		{
			LOG_ERROR("init decoder faild!!!");
			return -1;
		}
		else
		{
			LOG_INFO("init decoder success!!!");
			vYcAvDecodersList.emplace_back(std::move(pYcAvDecoder));
		}
	}

	//测试播放 ffplay -pixel_format yuv420p -video_size 1280x720 -framerate 25 .\testvideo3_1280x720_yuv420p.yuv
	FILE* pVideoFile = nullptr;
	fopen_s(&pVideoFile,"./testvideo3_1280x720_yuv420p.yuv", "wb");
	//测试播放 ffplay -f f32le -ar 44100 -ac 2 .\testvideo3.pcm
	FILE* pAudioFile = nullptr;
	fopen_s(&pAudioFile,"./testvideo3.pcm", "wb");

	YCQueue<YCAVPacket> packetQueue;

	while (1)
	{
		std::shared_ptr<YCAVPacket> pYcAvPacket = std::make_shared<YCAVPacket>();
		ret = reader.Read(pYcAvPacket.get());

		if (ret != YCRet::YCRet_OK)
		{
			if (ret == YCRet::YCRet_EOF)
			{
				LOG_INFO("read frame eof");
			}
			else
			{
				LOG_ERROR("read frame error!!!");
			}
			break;
		}
		else
		{
			//LOG_INFO("read frame success");
			int streamIndex = pYcAvPacket->GetStreamIndex();
			YCAVDecoder *pCurDecoder{ nullptr };
			for (const auto& decoder : vYcAvDecodersList)
			{
				if (decoder->GetDecoderIndex() == streamIndex)
				{
					pCurDecoder = decoder.get();
					break;
				}
			}
			if (pCurDecoder)
			{
				ret = pCurDecoder->SendPacket(pYcAvPacket.get());
				if (ret != YCRet::YCRet_OK)
				{
					// 发送包失败，可能是因为数据不足或解码器状态不正确 继续发送下一个
					continue;
				}
				while (1)
				{
					YCAVFrame ycAvFrame;
					ret = pCurDecoder->ReceiveFrame(&ycAvFrame);
					if (ret != YCRet::YCRet_OK)
					{
						if (ret == YCRet::YCRet_NeedMorePacket)
						{
							 // 需要更多数据，跳出循环等待下一次发送
							//LOG_INFO("decoder need more packet, streamIndex:{0}", { std::to_string(streamIndex) });
						}
						else if (ret == YCRet::YCRet_EOF)
						{
							// 解码器已完成处理
							LOG_INFO("decoder receive eof, streamIndex:{0}", { std::to_string(streamIndex) });
						}
						else 
						{
							LOG_ERROR("receive frame error!!!");
						}
						break;
					}
					else
					{
						LOG_INFO("receive frame success, streamIndex:{0}", { std::to_string(streamIndex) });
						// 处理解码后的帧（ycAvFrame）
						if (iVideoStreamIndex == pYcAvPacket->GetStreamIndex())
						{
							ycAvFrame.PrintVideoInfo();
							int width = ycAvFrame.GetW();
							int height = ycAvFrame.GetH();	

							//注意这里是申请数组空间
							std::unique_ptr<unsigned char[]> pYbuffer = std::make_unique<unsigned char[]>(width * height);
							std::unique_ptr<unsigned char[]> pUbuffer = std::make_unique<unsigned char[]>(width / 2 * height / 2);
							std::unique_ptr<unsigned char[]> pVbuffer = std::make_unique<unsigned char[]>(width / 2 * height / 2);

							ycAvFrame.GetY(pYbuffer.get());
							ycAvFrame.GetU(pUbuffer.get());
							ycAvFrame.GetV(pVbuffer.get());

							fwrite(pYbuffer.get(), width * height,1,pVideoFile);
							fwrite(pUbuffer.get(), width/2 * height/2,1,pVideoFile);
							fwrite(pVbuffer.get(), width/2 * height/2,1,pVideoFile);
						}
						else if (iAudioStreamIndex == pYcAvPacket->GetStreamIndex())
						{
							ycAvFrame.PrintAudioInfo();

							std::unique_ptr<unsigned char[]> pPcmBuffer = std::make_unique<unsigned char[]>(ycAvFrame.GetPcmSize());
							ycAvFrame.GetPcm(pPcmBuffer.get());
							fwrite(pPcmBuffer.get(), ycAvFrame.GetPcmSize(), 1, pAudioFile);
						}
					}
				}
			}
		}
		
		packetQueue.Push(std::move(pYcAvPacket));
	}

	//当读取packet结束后 再次调用解码器处理解码器中的缓存packet
	for (const auto& decoder : vYcAvDecodersList)
	{
		if (decoder)
		{
			ret = decoder->SendPacket(nullptr);
			if (ret != YCRet::YCRet_OK)
			{
				// 发送包失败，可能是因为数据不足或解码器状态不正确 继续发送下一个
				continue;
			}
			while (1)
			{
				YCAVFrame ycAvFrame;
				ret = decoder->ReceiveFrame(&ycAvFrame);
				if (ret != YCRet::YCRet_OK)
				{
					if (ret == YCRet::YCRet_NeedMorePacket)
					{
						// 需要更多数据，跳出循环等待下一次发送
						//LOG_INFO("decoder need more packet, streamIndex:{0}", { std::to_string(decoder->GetDecoderIndex()) });
					}
					else if (ret == YCRet::YCRet_EOF)
					{
						// 解码器已完成处理
						LOG_INFO("decoder receive eof, streamIndex:{0}", { std::to_string(decoder->GetDecoderIndex()) });
					}
					else
					{
						LOG_ERROR("receive frame error!!!");
					}
					break;
				}
				else
				{
					if (decoder->GetDecoderIndex() == iVideoStreamIndex)
					{
						// 处理最后解码的帧
						ycAvFrame.PrintVideoInfo();
						int width = ycAvFrame.GetW();
						int height = ycAvFrame.GetH();

						std::unique_ptr<unsigned char[]> pYbuffer = std::make_unique<unsigned char[]>(width * height);
						std::unique_ptr<unsigned char[]> pUbuffer = std::make_unique<unsigned char[]>(width / 2 * height / 2);
						std::unique_ptr<unsigned char[]> pVbuffer = std::make_unique<unsigned char[]>(width / 2 * height / 2);

						ycAvFrame.GetY(pYbuffer.get());
						ycAvFrame.GetU(pUbuffer.get());
						ycAvFrame.GetV(pVbuffer.get());

						fwrite(pYbuffer.get(), width* height, 1, pVideoFile);
						fwrite(pUbuffer.get(), width / 2 * height / 2, 1, pVideoFile);
						fwrite(pVbuffer.get(), width / 2 * height / 2, 1, pVideoFile);
					}
					else if (decoder->GetDecoderIndex() == iAudioStreamIndex)
					{
						ycAvFrame.PrintAudioInfo();

						std::unique_ptr<unsigned char[]> pPcmBuffer = std::make_unique<unsigned char[]>(ycAvFrame.GetPcmSize());
						ycAvFrame.GetPcm(pPcmBuffer.get());
						fwrite(pPcmBuffer.get(), ycAvFrame.GetPcmSize(), 1, pAudioFile);
					}
					
				}
			}
		}
	}
	
	reader.Close();
	fclose(pVideoFile);
	fclose(pAudioFile);

	//清空packetQueue
	LOG_INFO("Clearing packet queue size:{0}", {std::to_string(packetQueue.Size())});
	packetQueue.Clear();

	//清理解码器
	vYcAvDecodersList.clear();

	return 0;
}

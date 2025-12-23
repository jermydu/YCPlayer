#ifndef _YCAV_H
#define _YCAV_H

namespace YCLIB{
	enum class YCRet;

	//避免外部引用这个文件的时候 需要指定ffmpeg头文件
	//avpacket 封装
	class YCAVPacket{	
	public:
		YCAVPacket();
		virtual ~YCAVPacket();
		int GetStreamIndex() const; //获取流索引
	private:
		//允许 Reader YCAVDecoder 访问内部实现
		friend class YCAVReader;
		friend class YCAVDecoder;
		struct Imp;
		Imp* imp{nullptr};

	};

	//AVFrame的封装
	class YCAVFramePrivate;
	class YCAVFrame{
	public:
		YCAVFrame();
		~YCAVFrame();
		YCRet PrintVideoInfo() const;
		int GetW() const;
		int GetH() const;
		void GetY(unsigned char* y) const;	
		void GetU(unsigned char* u) const;
		void GetV(unsigned char* v) const;

		YCRet PrintAudioInfo() const;
		int GetPcmSize()const; //获取pcm数据大小
		void GetPcm(unsigned char* pcm) const; //获取pcm数据
	public:
		YCAVFramePrivate* imp{ nullptr }; 
	};

	//avformatcontext 封装
	class YCAVStream;
	class YCAVReader {
	public:
		YCAVReader();
		virtual ~YCAVReader();

		YCRet Open(const char* path);
		YCRet Close();

		int GetStreamCount() const;
		YCRet GetStream(int index, YCAVStream* pYcAvStream) const;
		YCRet Read(YCAVPacket* packet);
		int GetVideoStreamIndex() const;
		int GetAudioStreamIndex() const;
		
	private:
		//观察指针  智能指针放到.cpp中
		struct Imp;
		Imp* imp{ nullptr };
	};

	//avstream 封装
	class YCAVStreamPrivate;
	class YCAVStream{
	public:
		YCAVStream();
		virtual ~YCAVStream();
	public:
		int streamIndex{ -1 }; //流索引
		YCAVStreamPrivate* imp{ nullptr };
	};

	//avcodec 封装
	class YCAVDecoderPrivate;
	class YCAVDecoder{
	public:
		YCAVDecoder();
		virtual ~YCAVDecoder();

		YCRet Init(YCAVStream* pYcAvStream);
		YCRet Release();
		YCRet SendPacket(YCAVPacket* pYcAvPacket);
		YCRet ReceiveFrame(YCAVFrame* pYcAvFrame);
		//获取解码器索引
		int GetDecoderIndex() const;
	private:
		YCAVDecoderPrivate* imp{ nullptr };
		int decoderIndex{ -1 }; //解码器索引
	};
}

#endif //_YCAV_H
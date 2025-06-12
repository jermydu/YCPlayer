#ifndef _YCAV_H
#define _YCAV_H

namespace YCAV 
{
	//避免外部引用这个文件的时候 需要指定ffmpeg头文件
	enum class YCRet;

	class YCAVPacketPrivate;
	class YCAVPacket
	{	
	public:
		YCAVPacket();
		virtual ~YCAVPacket();
	public:
		YCAVPacketPrivate* pAvPacketImp{nullptr};

	};

	class YCAVFormatContextPrivate;
	class YCAVReader 
	{
	public:
		YCAVReader();
		virtual ~YCAVReader();

		YCRet Open(const char* path);
		YCRet Close();

		YCRet Read(YCAVPacket* packet);
		
	private:
		YCAVFormatContextPrivate* pAVFormatContextImp{ nullptr };
	};
}

#endif //_YCAV_H
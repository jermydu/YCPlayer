#ifndef _YCCOMMONDEF_H
#define _YCCOMMONDEF_H

extern "C" {
#include "libavformat/avformat.h"
}
#include "spdlog/spdlog.h"

#include <string>
using namespace std;

enum class YCRet {
	YCRet_TryAgain = 1,		   //重试中  比如数据正在准备中
	YCRet_EOF = 2,             //读到文件或者流末尾
	YCRet_NotSeekable = 3,     //文件或者流不能seek
	YCRet_NeedMorePacket = 4,  //需要更多avpacket包来编码
	YCRet_Caching = 5,		   //缓存中

	//c 接口
	YCRet_OK = 0,               //成功 没有错误
	YCRet_NotExist = -1,		//文件或流没有退出
	YCRet_InvalidParam = -2,    //函数参数无效
	YCRet_NotOpened = -3,		//文件或流没有被打开
	YCRet_NotSupported = -4,    //不支持的文件类型

	//ffmpeg context
	YCRet_NotDecoderContext = -11,         //不是解码器context
	YCRet_NotEncoderContext = -12,         //不是编码器context
	YCRet_NotDemuxerContext = -13,         //不是解封装context
	YCRet_NotMuxerContext = -14,           //不是封装contex
	YCRet_NotVideoConvertorContext = -15,  //不是视频转码器context
	YCRet_NotAudioResamplerContext = -16,  //不是音频重采样context

	YCRet_CreateContextFailed = -101,    //创建context错误
	YCRet_OpenFileFailed = -102,         //打开文件或流失败
	YCRet_OpenFileTimeOut = -103,        //打开文件或流超时
	YCRet_FileNoStream = -104,           //没有流
	YCRet_InvalidStreamIndex = -105,     //流index无效
	YCRet_ReadNotStarted = -106,         //开始read之前读取packet
	YCRet_ReadPacketTimeOut = -107,      //读取packet超时
	YCRet_ReadPacketIOError = -108,      //读取packet IO error
	YCRet_ContextAlreadyCreated = -109,  //context已经创建
	YCRet_WritePacketFailed = -110,      //写packet到封装器失败
	YCRet_ReadPacketOtherError = -111,	 //读取packet时的未知错误

};

string GetFfmpegErrorString(int err) {
	char tmp[AV_ERROR_MAX_STRING_SIZE] = { 0 };
	av_strerror(err, tmp, AV_ERROR_MAX_STRING_SIZE);
	string ret = "";
	ret.append(tmp, AV_ERROR_MAX_STRING_SIZE);
	return ret;
}

#endif  //_YCCOMMONDEF_H
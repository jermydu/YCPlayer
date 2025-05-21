// YCPlayer.cpp: 定义应用程序的入口点。
//

#include "YCPlayer.h"
#include "spdlog/spdlog.h"

using namespace std;

int main()
{
	spdlog::info("Welcome to spdlog version {}.{}.{}  !", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR,
		SPDLOG_VER_PATCH);
	return 0;
}

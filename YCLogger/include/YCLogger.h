#pragma once

#include "YCLoggerConfig.h"

//•	Pimpl + 非模板接口：所有实现细节都在 cpp 文件，头文件只暴露非模板接口，保证二进制兼容和实现隐藏。
//•	参数格式：通过 std::vector<std::string> 传递参数，宏展开时自动转为 initializer_list，支持任意数量参数。
//•	格式化：简单实现了 {0}、{1}... 占位符替换，满足大部分日志需求。
//•	类型安全：调用时需手动将参数转为 std::string，如 std::to_string(iErrorCode)。
namespace YCLIB {

class Logger 
{
private:
    class Impl;  // 声明一个不透明的实现类
    std::unique_ptr<Impl> impl_;

    Logger();                    // 私有构造
    ~Logger();                   // 私有析构
public:

    // 全局单例
    static Logger& instance();

    void Initialize(const std::string& log_file,  bool async = true, size_t queue_size = 8192, size_t thread_count = 1);

    void Shutdown();

    void SetLogLevel(LoggerLevel level);

    //使用非模板接口，参数全部转为std::string
    //使用模版必须写在.h文件 这样会暴露spdlog
    void LogDebug(const char* function, int line, const std::string& fmt, const std::vector<std::string>& args = {});
    void LogInfo(const char* function, int line, const std::string& fmt, const std::vector<std::string>& args = {});
    void LogWarn(const char* function, int line, const std::string& fmt, const std::vector<std::string>& args = {});
    void LogError(const char* function, int line,const std::string& fmt, const std::vector<std::string>& args = {});

};

// 使用#define模式简化调用 __VA_ARGS__ 表示传递给宏的可变参数，## 用于在参数为空时去掉前面的逗号。
#define LOG_DEBUG(fmt, ...) YCLIB::Logger::instance().LogDebug(__FUNCTION__, __LINE__,fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  YCLIB::Logger::instance().LogInfo(__FUNCTION__,__LINE__,fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  YCLIB::Logger::instance().LogWarn(__FUNCTION__,__LINE__,fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) YCLIB::Logger::instance().LogError(__FUNCTION__,__LINE__,fmt, ##__VA_ARGS__)
}  // namespace YCLIB

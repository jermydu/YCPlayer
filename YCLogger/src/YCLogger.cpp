#include "YCLogger.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/async.h>
#include <spdlog/async_logger.h>

namespace YCLIB
{

    class Logger::Impl
    {
    public:
        std::shared_ptr<spdlog::logger> CreateLogger(const std::string &log_file, bool async, size_t queue_size, size_t thread_count)
        {
            // 日志文件名不带扩展名
            std::string base_name = log_file.substr(0, log_file.find_last_of('.'));
            std::string ext = log_file.substr(log_file.find_last_of('.'));

            // 每天一个文件，保留7天
            auto daily_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(base_name + "_daily" + ext, 0, 0, false, 7);

            // 单文件最大5MB，最多保留3个
            //auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(base_name + "_rotating" + ext, 1024 * 1024 * 5, 3);

            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

            if (async)
            {
                // 初始化线程池
                thread_pool_ = std::make_shared<spdlog::details::thread_pool>(queue_size, thread_count);
                auto logger = std::make_shared<spdlog::async_logger>(
                    "YCLogger",
                    spdlog::sinks_init_list{console_sink, daily_sink,/*rotating_sink*/},
                    thread_pool_,
                    spdlog::async_overflow_policy::block);
                logger->set_level(spdlog::level::info);
                logger->set_pattern("[%Y-%m-%d %H:%M:%S,%e] [%^%l%$] [thread:%t] %v");
                return logger;
            }
            else
            {
                auto logger = std::make_shared<spdlog::logger>("YCLogger", spdlog::sinks_init_list{console_sink, daily_sink,/*rotating_sink*/});
                logger->set_level(spdlog::level::info);
                logger->set_pattern("[%Y-%m-%d %H:%M:%S,%e] [%^%l%$] [thread:%t] %v");

                return logger;
            }
        }

        void SetLogLevel(LoggerLevel level)
        {
            switch (level)
            {
            case LoggerLevel::Debug:
                logger_->set_level(spdlog::level::debug);
                break;
            case LoggerLevel::Info:
                logger_->set_level(spdlog::level::info);
                break;
            case LoggerLevel::Warn:
                logger_->set_level(spdlog::level::warn);
                break;
            case LoggerLevel::Error:
                logger_->set_level(spdlog::level::err);
                break;
            default:
                logger_->set_level(spdlog::level::info);
                break;
            }
        }

        std::shared_ptr<spdlog::logger> GetLogger() const
        {
            return logger_;
        }

        // 简单的字符串格式化：用 {0} {1} ... 占位符替换
        static std::string Format(const std::string &fmt, const std::vector<std::string> &args)
        {
            std::string result = fmt;
            for (size_t i = 0; i < args.size(); ++i)
            {
                std::string placeholder = "{" + std::to_string(i) + "}";
                size_t pos = 0;
                while ((pos = result.find(placeholder, pos)) != std::string::npos)
                {
                    result.replace(pos, placeholder.length(), args[i]);
                    pos += args[i].length();
                }
            }

            return result;
        }

        std::shared_ptr<spdlog::logger> logger_;
        std::shared_ptr<spdlog::details::thread_pool> thread_pool_; // 如果需要异步日志，可以使用线程池
    };

    // 单例定义
    Logger& Logger::instance() 
    {
        static Logger inst;
        return inst;
    }

    Logger::Logger() : impl_(std::make_unique<Impl>()) 
    {

    }

    Logger::~Logger()
    {
        Shutdown();
    }

    void Logger::Initialize(const std::string &log_file, bool async, size_t queue_size, size_t thread_count)
    {
        impl_->logger_ = impl_->CreateLogger(log_file, async, queue_size, thread_count);
    }

    void Logger::Shutdown()
    {
        if (impl_)
        {
            impl_->logger_->flush();
            spdlog::drop("YCLogger");
            impl_.reset();
        }
    }

    void Logger::SetLogLevel(LoggerLevel level)
    {
        if (impl_)
        {
            impl_->SetLogLevel(level);
        }
    }
    void Logger::LogDebug(const char *function, int line, const std::string &fmt, const std::vector<std::string> &args)
    {
        if (!impl_)
            return;
        std::string msg = "[" + std::string(function) + ":" + std::to_string(line) + "] " + Impl::Format(fmt, args);
        impl_->logger_->debug(msg);
    }
    void Logger::LogInfo(const char *function, int line, const std::string &fmt, const std::vector<std::string> &args)
    {
        if (!impl_)
            return;
        std::string msg = "[" + std::string(function) + ":" + std::to_string(line) + "] " + Impl::Format(fmt, args);
        impl_->logger_->info(msg);
    }
    void Logger::LogWarn(const char *function, int line, const std::string &fmt, const std::vector<std::string> &args)
    {
        if (!impl_)
            return;
        std::string msg = "[" + std::string(function) + ":" + std::to_string(line) + "] " + Impl::Format(fmt, args);
        impl_->logger_->warn(msg);
    }
    void Logger::LogError(const char *function, int line, const std::string &fmt, const std::vector<std::string> &args)
    {
        if (!impl_)
            return;
        std::string msg = "[" + std::string(function) + ":" + std::to_string(line) + "] " + Impl::Format(fmt, args);
        impl_->logger_->error(msg);
    }
} // namespace YCLIB
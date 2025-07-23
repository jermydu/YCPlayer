#pragma once
#include <memory>
#include <vector>
#include <string>
#include <utility> // for std::forward

namespace YCLIB {

enum class LoggerLevel
{
    Debug,
    Info,
    Warn,
    Error,
};

}  // namespace YCLIB
#pragma once
#include <reusable_synth/utils/logger.hpp>

/**
 * @brief Helper class to wrap a single logger that can be accessed
 * anywhere.
 *
 */
class LoggerSingleton
{
public:
    constexpr static int nLogs = 20;
    constexpr static int logLen = 200;
    static auto get() -> Logger<nLogs, logLen>*
    {
        static Logger<nLogs, logLen> instance;
        return &instance;
    }

    LoggerSingleton(const LoggerSingleton&) = delete;
    auto operator=(const LoggerSingleton&) -> LoggerSingleton& = delete;
    LoggerSingleton(LoggerSingleton&&) = delete;
    auto operator=(LoggerSingleton&&) -> LoggerSingleton& = delete;
    ~LoggerSingleton() = default;

private:
    LoggerSingleton() = default;
};
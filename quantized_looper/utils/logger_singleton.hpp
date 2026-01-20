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
    static Logger<nLogs, logLen>* get()
    {
        static Logger<nLogs, logLen> instance;
        return &instance;
    }

    LoggerSingleton(LoggerSingleton const&) = delete;
    void operator=(LoggerSingleton const&) = delete;

private:
    LoggerSingleton() {}
};
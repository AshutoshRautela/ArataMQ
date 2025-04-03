#include "logger.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace aratamq
{
    Logger *Logger::instance = nullptr;

    Logger::Logger(std::string name, std::string logDir, std::string logFile, bool truncate)
    {
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
            logDir + "/" + logFile,
            truncate
        );

        file_sink->set_level(spdlog::level::info);
        file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] %v");

        this->logger = std::make_shared<spdlog::logger>(name, file_sink);
        this->logger->set_level(spdlog::level::info);
        this->logger->flush_on(spdlog::level::info);

         // Force immediate flush
        this->logger->flush();
    }

    void Logger::Initialize(const std::string &name, const std::string &logDir, const std::string &logFile, const bool &truncate)
    {
        if (!instance)
        {
            instance = new Logger(name, logDir, logFile, truncate);
        }
    }

    Logger &Logger::Instance()
    {
        if (!instance)
        {
            throw std::runtime_error("Logger not initialized");
        }
        return *instance;
    }

    void Logger::cleanup()
    {
        if (instance)
        {
            delete instance;
            instance = nullptr;
        }
    }
    Logger::~Logger()
    {
        spdlog::drop_all();
    }
}
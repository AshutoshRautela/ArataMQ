#include "logger.h"

namespace aratamq {
    Logger* Logger::instance = nullptr;

    Logger::Logger(std::string logDir, std::string logFile) {
        logger = spdlog::basic_logger_mt("ArataMQ", logDir + "/" + logFile, true);
    }

    void Logger::Initialize(const std::string& logDir, const std::string& logFile) {
        if (!instance) {    
            instance = new Logger(logDir, logFile);
        }
    }

    Logger& Logger::Instance() {
        if (!instance) {
            throw std::runtime_error("Logger not initialized");
        }
        return *instance;
    }

    void Logger::cleanup() {
        if (instance) {
            delete instance;
            instance = nullptr;
        }
    }
    Logger::~Logger() {
        spdlog::drop_all();
    }
}
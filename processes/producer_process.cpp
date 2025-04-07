#include "core/broker/broker.h"
#include "utils/logger.h"
#include "utils/utils.h"
#include "config/aratamqconfig.h"

int main()
{
    armq::ArataMQConfig config;
    aratamq::Logger::Initialize("ArataMQ_Producer", config.logFileDir, config.logFile, false);
    aratamq::Logger::Instance().Info("Connected ArataMQ Producer");

    aratamq::Logger::Instance().Info("Shutting down ArataMQ Producer");
    aratamq::Logger::cleanup();
    return 0;
}
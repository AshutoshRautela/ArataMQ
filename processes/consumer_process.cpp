#include "core/broker/broker.h"
#include "utils/logger.h"
#include "utils/utils.h"
#include "config/aratamqconfig.h"

int main() {
    armq::ArataMQConfig config;
    aratamq::Logger::Initialize("ArataMQ_Consumer", config.logFileDir, config.logFile, false);
    aratamq::Logger::Instance().Info("Connected ArataMQ Consumer");

    //armq::Broker broker(config.queueFileDir, config.queueFile);
    aratamq::Logger::Instance().Info("Shutting down ArataMQ Consumer");
    aratamq::Logger::cleanup();
    return 0;
}
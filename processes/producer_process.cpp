#include "core/broker/broker.h"
#include "utils/logger.h"
#include "utils/utils.h"
#include "config/aratamqconfig.h"

armq::Message PrepareSampleMessage()
{
    armq::Metadata metadata;
    metadata.messageId = "1234567890";
    metadata.timestamp = "2021-01-01 12:00:00";
    metadata.source = "first-source";
    metadata.destination = "first-destination";
    metadata.type = "type";

    armq::Headers headers;
    headers.contentType = "application/json";
    headers.contentEncoding = "utf-8";
    headers.customHeaders = {};

    nlohmann::json payload;
    payload["message1"] = "Hey!";
    payload["timestamp"] = armq::GetCurrentTimestamp();

    return armq::Message(std::move(metadata), std::move(headers), std::move(payload));
}

int main()
{
    armq::ArataMQConfig config;
    aratamq::Logger::Initialize("ArataMQ_Producer", config.logFileDir, config.logFile, false);
    aratamq::Logger::Instance().Info("Connected ArataMQ Producer");

    armq::Broker broker(config.queueFileDir, config.queueFile);
    int timeOut = 10;
    while (timeOut > 0)
    {
        broker.Produce(PrepareSampleMessage());
        std::this_thread::sleep_for(std::chrono::seconds(1));
        timeOut--;
    }

    aratamq::Logger::Instance().Info("Shutting down ArataMQ Producer");
    aratamq::Logger::cleanup();
    return 0;
}
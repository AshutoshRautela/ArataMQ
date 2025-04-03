#include <iostream>
#include <memory>
#include "nlohmann/json.hpp"

#include "config/aratamqconfig.h"
#include "core/broker/broker.h"
#include "core/message/message.h"
#include "utils/logger.h"
#include "utils/utils.h"

armq::Message PrepareSampleMessage() {
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
    try
    {
        armq::ArataMQConfig config;
        aratamq::Logger::Initialize(config.logFileDir, config.logFile);
        aratamq::Logger::Instance().Info("Starting ArataMQ");

        armq::Broker broker(config.queueFileDir, config.queueFile);
        broker.Produce(PrepareSampleMessage());

        aratamq::Logger::Instance().Info("Shutting down ArataMQ");
        aratamq::Logger::cleanup();
        return 0;
    }
    catch (const std::exception &e)
    {
        aratamq::Logger::Instance().Error("Error {}",std::string(e.what()));
        return 1;
    }
}
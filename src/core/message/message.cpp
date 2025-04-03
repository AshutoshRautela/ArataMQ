#include "message.h"

namespace armq
{
    Message::Message(Metadata &&metadata, Headers &&headers, nlohmann::json &&payload)
    {
        if (metadata.messageId.empty() || metadata.timestamp.empty() || metadata.source.empty() || metadata.destination.empty() || metadata.type.empty())
        {
            throw std::runtime_error("Invalid message metadata");
        }
        this->metadata = metadata;
        this->headers = headers;
        this->payload = payload;
    }

    std::string Message::Stringify() const
    {
        nlohmann::json jsonData;
        jsonData["metadata"] = {
            {"messageId", this->metadata.messageId},
            {"timestamp", this->metadata.timestamp},
            {"source", this->metadata.source},
            {"destination", this->metadata.destination},
            {"type", this->metadata.type}};
        jsonData["headers"] = {
            {"contentType", this->headers.contentType},
            {"contentEncoding", this->headers.contentEncoding},
            {"customHeaders", this->headers.customHeaders}};
        jsonData["payload"] = {
            {"data", this->payload}};
        return jsonData.dump();
    }

    Message Message::Parse(std::string &json)
    {
        try
        {
            nlohmann::json jsonData = nlohmann::json::parse(json);

            Metadata metadata;
            metadata.messageId = jsonData["metadata"]["messageId"];
            metadata.timestamp = jsonData["metadata"]["timestamp"];
            metadata.source = jsonData["metadata"]["source"];
            metadata.destination = jsonData["metadata"]["destination"];
            metadata.type = jsonData["metadata"]["type"];

            Headers headers;
            headers.contentType = jsonData["headers"]["contentType"];
            headers.contentEncoding = jsonData["headers"]["contentEncoding"];
            headers.customHeaders = jsonData["headers"]["customHeaders"];

            nlohmann::json payload = jsonData["payload"]["data"];

            return Message(std::move(metadata), std::move(headers), std::move(payload));
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error("Failed to parse message: " + std::string(e.what()));
        }
    }

    Message::~Message()
    {
        this->metadata = {};
        this->headers = {};
        this->payload = {};
    }
}
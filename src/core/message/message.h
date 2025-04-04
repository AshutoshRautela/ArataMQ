#ifndef AR_MESSAGE_H
#define AR_MESSAGE_H

#include <string>
#include <nlohmann/json.hpp>

namespace armq
{
    struct Metadata
    {
        std::string messageId;
        std::string timestamp;
        std::string source;
        std::string destination;
        std::string type;

        bool operator==(const Metadata& other) const {
            return messageId == other.messageId &&
                   timestamp == other.timestamp &&
                   source == other.source &&
                   destination == other.destination &&
                   type == other.type;
        }
    };

    struct Headers
    {
        std::string contentType;
        std::string contentEncoding;
        std::map<std::string, std::string> customHeaders;

        bool operator==(const Headers& other) const {
            return contentType == other.contentType &&
                   contentEncoding == other.contentEncoding &&
                   customHeaders == other.customHeaders;
        }
    };

    class Message
    {
    private:
        nlohmann::json payload;
        Metadata metadata;
        Headers headers;

    public:
        std::string Stringify() const;
        static Message Parse(std::string &);

        const inline Metadata &getMetadata() const { return this->metadata; }
        const inline Headers &getHeaders() const { return this->headers; }
        const inline nlohmann::json &getPayload() const { return this->payload; }

        bool operator==(const Message& other) const {
            return metadata == other.metadata &&
                   headers == other.headers &&
                   payload == other.payload;
        }

        Message() = delete;
        Message(const Message&) = default;            // Copy constructor
        Message(Message&&) = default;                 // Move constructor
        Message& operator=(const Message&) = default; // Copy assignment
        Message& operator=(Message&&) = default;      // Move assignment
        
        Message(Metadata&&, Headers&&, nlohmann::json&&);
        ~Message();
    };
}

#endif
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
    };

    struct Headers
    {
        std::string contentType;
        std::string contentEncoding;
        std::map<std::string, std::string> customHeaders;
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

        Message() = delete;
        Message(Metadata&&, Headers&&, nlohmann::json&&);
        ~Message();
    };
}

#endif
#ifndef AR_IEXCHANGE_H
#define AR_IEXCHANGE_H
#include <string>
#include <memory>
#include "../queue/queue.h"

namespace armq
{

    enum class ExchangeType {
        DIRECT,
        FANOUT,
        TOPIC,
        HEADERS
    };

    class IExchange
    {
    protected:
        std::string m_name;
        ExchangeType m_type;

    public:
        IExchange(const std::string &name, ExchangeType type): m_name(name), m_type(type) {}
        virtual ~IExchange() = default;

        virtual void BindQueue(std::shared_ptr<Queue> queue, const std::optional<std::string> &bindingKey) = 0;
        virtual void UnbindQueue(std::shared_ptr<Queue> queue, const std::optional<std::string> &bindingKey) = 0;
        virtual void RouteMessage(const Message &message, const std::optional<std::string> &routingKey) = 0;

        const inline std::string &GetName() const { return m_name; }
        const inline ExchangeType GetType() const { return m_type; }
    };
}

#endif

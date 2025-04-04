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

        const inline std::string &getName() const { return m_name; }
        const inline ExchangeType getType() const { return m_type; }
    };
}

#endif

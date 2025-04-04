#include "direct_exchange.h"

namespace armq
{
    DirectExchange::DirectExchange(const std::string &name) : IExchange(name, ExchangeType::DIRECT) {}

    void DirectExchange::BindQueue(const std::string &routingKey, std::shared_ptr<Queue> queue)
    {
        if (this->m_routingTable.find(routingKey) != this->m_routingTable.end())
            throw std::runtime_error("Routing key '" + routingKey + "' already bound to a queue in exchange '" + m_name + "'");
        this->m_routingTable[routingKey] = queue;
    }

    void DirectExchange::UnbindQueue(const std::string &routingKey, std::shared_ptr<Queue> queue)
    {
        if (this->m_routingTable.find(routingKey) != this->m_routingTable.end())
            this->m_routingTable.erase(routingKey);
    }

    void DirectExchange::RouteMessage(const std::string &routingKey, const Message &message)
    {
        if (this->m_routingTable.find(routingKey) == this->m_routingTable.end())
            throw std::runtime_error("No queue bound to routing key '" + routingKey + "' in exchange '" + m_name + "'");
        this->m_routingTable[routingKey]->Enqueue(message);
    }
}

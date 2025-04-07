#include "direct_exchange.h"
#include "utils/utils.h"

namespace armq
{
    DirectExchange::DirectExchange(const std::string &name) : IExchange(name, ExchangeType::DIRECT) {}

    void DirectExchange::BindQueue(std::shared_ptr<Queue> queue, const std::optional<std::string> &routingKey)
    {
        if (!routingKey.has_value())
            throw std::invalid_argument("Routing key is required");
        ValidateRoutingKey(routingKey.value());
        if (this->m_routingTable.find(routingKey.value()) != this->m_routingTable.end())
            throw std::runtime_error("Routing key '" + routingKey.value() + "' already bound to a queue in exchange '" + m_name + "'");
        this->m_routingTable[routingKey.value()] = queue;
    }

    void DirectExchange::UnbindQueue(std::shared_ptr<Queue> queue, const std::optional<std::string> &routingKey)
    {
        if (!routingKey.has_value())
            throw std::invalid_argument("Routing key is required");
        ValidateRoutingKey(routingKey.value());

        if (this->m_routingTable.find(routingKey.value()) == this->m_routingTable.end())
            throw std::runtime_error("Routing key '" + routingKey.value() + "' not bound to any queue in exchange '" + m_name + "'");

        if (this->m_routingTable[routingKey.value()] != queue)
            throw std::runtime_error("Queue '" + queue->getName() + "' not bound to routing key '" + routingKey.value() + "' in exchange '" + m_name + "'");

        this->m_routingTable.erase(routingKey.value());
    }

    void DirectExchange::RouteMessage(const Message &message, const std::optional<std::string> &routingKey)
    {
        if (!routingKey.has_value())
            throw std::invalid_argument("Routing key is required");
        ValidateRoutingKey(routingKey.value());

        if (this->m_routingTable.find(routingKey.value()) == this->m_routingTable.end())
            throw std::runtime_error("No queue bound to routing key '" + routingKey.value() + "' in exchange '" + m_name + "'");
        this->m_routingTable[routingKey.value()]->Enqueue(message);
    }

    std::shared_ptr<Queue> DirectExchange::GetQueue(const std::string &routingKey)
    {
        if (this->m_routingTable.find(routingKey) == this->m_routingTable.end())
            throw std::runtime_error("No queue bound to routing key '" + routingKey + "' in exchange '" + m_name + "'");
        return this->m_routingTable[routingKey];
    }
}

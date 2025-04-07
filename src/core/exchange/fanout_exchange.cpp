#include "fanout_exchange.h"

namespace armq
{
    FanoutExchange::FanoutExchange(const std::string &name) : IExchange(name, ExchangeType::FANOUT) {}

    void FanoutExchange::BindQueue(std::shared_ptr<Queue> queue, const std::optional<std::string> &bindingKey)
    {
        if (bindingKey.has_value())
            throw std::invalid_argument("Fanout exchange does not support binding keys");
        this->m_queues.insert(queue);
    }

    void FanoutExchange::UnbindQueue(std::shared_ptr<Queue> queue, const std::optional<std::string> &bindingKey)
    {
        if (bindingKey.has_value())
            throw std::invalid_argument("Fanout exchange does not support binding keys");
        if (!this->m_queues.count(queue))
            throw std::runtime_error("Queue not bound to exchange");
        this->m_queues.erase(queue);
    }

    void FanoutExchange::RouteMessage(const Message &message, const std::optional<std::string> &routingKey)
    {
        if (routingKey.has_value())
            throw std::invalid_argument("Fanout exchange does not support routing keys");
        for (std::shared_ptr<Queue> queue : this->m_queues)
            queue->Enqueue(message);
    }
}

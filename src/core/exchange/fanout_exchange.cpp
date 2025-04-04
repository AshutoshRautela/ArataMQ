#include "fanout_exchange.h"

namespace armq
{
    FanoutExchange::FanoutExchange(const std::string &name) : IExchange(name, ExchangeType::FANOUT) {}

    void FanoutExchange::BindQueue(std::shared_ptr<Queue> queue)
    {
        this->m_queues.insert(queue);
    }

    void FanoutExchange::UnbindQueue(std::shared_ptr<Queue> queue)
    {
        this->m_queues.erase(queue);
    }

    void FanoutExchange::RouteMessage(const Message &message)
    {
        for (std::shared_ptr<Queue> queue : this->m_queues)
            queue->Enqueue(message);
    }
}

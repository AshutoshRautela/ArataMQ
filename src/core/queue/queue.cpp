#include "queue.h"
#include <stdexcept>
#include "core/logger/logger.h"

namespace armq
{
    Queue::Queue(const std::string &name) : m_name(name) {}

    Queue::~Queue() {
        this->m_consumers.clear();
        this->m_consumersMap.clear();
        this->m_messages.clear();
    };

    void Queue::Enqueue(const Message &message)
    {
        m_messages.push(message);
        if (!m_messages.empty() && !this->m_consumers.empty()) {
            DispatchMessage();
        }
    }

    Message Queue::Dequeue()
    {
        if (m_messages.empty()) {
            throw std::runtime_error("Cannot dequeue from empty queue");
        }
        Message message = m_messages.front();
        m_messages.pop();
        return std::move(message);
    }

    void Queue::RegisterConsumer(std::shared_ptr<IConsumer> consumer)
    {
        if (this->m_consumersMap.count(consumer))
            throw std::runtime_error("Consumer already registered");

        this->m_consumers.push_back(consumer);
        this->m_consumersMap[consumer] = this->m_consumers.end();
        if (this->m_consumers.size() == 1)
            this->m_roundRobinIterator = this->m_consumers.begin();
    }

    void Queue::UnregisterConsumer(std::shared_ptr<IConsumer> consumer)
    {
        if (!this->m_consumersMap.count(consumer))
            throw std::runtime_error("Consumer not registered");

        if (this->m_consumersMap[consumer] == this->m_roundRobinIterator)
            this->m_roundRobinIterator = std::next(this->m_roundRobinIterator);

        this->m_consumers.erase(this->m_consumersMap[consumer]);
        this->m_consumersMap.erase(consumer);
    }

    void Queue::DispatchMessage()
    {
        if (m_messages.empty()) {
            throw std::runtime_error("Cannot dispatch message from empty queue");
        }
        if (this->m_consumers.empty()) {
            throw std::runtime_error("No consumers registered");
        }
        while (!m_messages.empty()) {
            Message message = m_messages.front();
            if (this->m_roundRobinIterator == this->m_consumers.end())
                this->m_roundRobinIterator = this->m_consumers.begin();
            int retries = 3;

            while (retries > 0) {
                if ((*this->m_roundRobinIterator)->Consume(message))
                {
                    m_messages.pop();
                    break;
                } else {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    aratamq::Logger::Instance().Warn("Consumer {} rejected message {}", (*this->m_roundRobinIterator)->GetName(), message.GetId());
                }
                retries--;
            }
            this->m_roundRobinIterator = std::next(this->m_roundRobinIterator);
        }
    }
}

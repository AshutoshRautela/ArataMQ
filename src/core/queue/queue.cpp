#include "queue.h"
#include <stdexcept>

namespace armq
{
    Queue::Queue(const std::string &name) : m_name(name) {}

    Queue::~Queue() = default;

    void Queue::Enqueue(const Message &message)
    {
        m_messages.push(message);
    }

    Message Queue::Dequeue()
    {
        if (m_messages.empty()) {
            throw std::runtime_error("Cannot dequeue from empty queue");
        }
        Message message = m_messages.front();
        m_messages.pop();
        return message;
    }
}

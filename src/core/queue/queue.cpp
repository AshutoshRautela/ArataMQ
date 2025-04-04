#include "queue.h"

namespace armq
{
    Queue::Queue(const std::string &name) : m_name(name) {}

    void Queue::Enqueue(const Message &message)
    {
        m_messages.push(message);
    }

    Message Queue::Dequeue()
    {
        Message message = m_messages.front();
        m_messages.pop();
        return message;
    }
}

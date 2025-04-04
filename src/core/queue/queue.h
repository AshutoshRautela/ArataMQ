#ifndef AR_QUEUE_H
#define AR_QUEUE_H

#include <queue>
#include <string>
#include "../message/message.h"
namespace armq
{
    class Queue
    {
        private:
            std::string m_name;
            std::queue<Message> m_messages;

        public:
            Queue(const std::string &name);
            ~Queue();

            void Enqueue(const Message &message);
            Message Dequeue();

            const inline std::string &getName() const { return m_name; }
            const inline bool IsEmpty() const { return m_messages.empty(); }
            const inline size_t Size() const { return m_messages.size(); }
    };
}

#endif

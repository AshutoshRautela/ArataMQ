#ifndef AR_QUEUE_H
#define AR_QUEUE_H

#include <queue>
#include <string>
#include <unordered_set>
#include <memory>
#include "core/message/message.h"
#include "core/consumer/iconsumer.h"

namespace armq
{
    class Queue
    {
        private:
            std::string m_name;
            std::queue<Message> m_messages;

            std::unordered_map<std::shared_ptr<IConsumer>, std::list<Message>::iterator> m_consumersMap;
            std::list<std::shared_ptr<IConsumer>> m_consumers;
            std::list<std::shared_ptr<IConsumer>>::iterator m_roundRobinIterator;

        public:
            Queue(const std::string &name);
            ~Queue();

            void Enqueue(const Message &message);
            Message Dequeue();

            void RegisterConsumer(std::shared_ptr<IConsumer> consumer);
            void UnregisterConsumer(std::shared_ptr<IConsumer> consumer);
            void DispatchMessage();


            const inline std::string &getName() const { return m_name; }
            const inline bool IsEmpty() const { return m_messages.empty(); }
            const inline size_t Size() const { return m_messages.size(); }
    };
}

#endif

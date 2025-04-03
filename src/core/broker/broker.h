#ifndef AR_BROKER_H
#define AR_BROKER_H

#include <string>
#include "../message/message.h"

namespace armq {
    class Broker {
        private:
            std::string m_queueDir;
            std::string m_queueFile;
            
            void initQueue();

        public:
            void Produce(const armq::Message&);
            void Consume(std::string);
            
            Broker(std::string, std::string);
            ~Broker();
    };
}

#endif  
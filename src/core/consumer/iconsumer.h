#ifndef I_CONSUMER_H
#define I_CONSUMER_H

#include <string>
#include "core/message/message.h"

class IConsumer {
public:
    virtual ~IConsumer() = default;
    virtual bool Consume(const Message& message) = 0;
};

#endif
#ifndef AR_FANOUT_EXCHANGE_H
#define AR_FANOUT_EXCHANGE_H

#include <unordered_set>
#include "iexchange.h"

namespace armq
{
    class FanoutExchange : public IExchange
    {
    private:
        std::unordered_set<std::shared_ptr<Queue>> m_queues;

    public:
        FanoutExchange(const std::string &name);

        void BindQueue(std::shared_ptr<Queue> queue);
        void UnbindQueue(std::shared_ptr<Queue> queue);
        void RouteMessage(const Message &message);  
    };
}

#endif

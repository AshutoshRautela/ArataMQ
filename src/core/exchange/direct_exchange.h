#ifndef AR_DIRECT_EXCHANGE_H
#define AR_DIRECT_EXCHANGE_H

#include "iexchange.h"
#include <unordered_map>
namespace armq
{
    class DirectExchange : public IExchange
    {
    private:
        std::unordered_map<std::string, std::shared_ptr<Queue>> m_routingTable;

    public:
        DirectExchange(const std::string &name);

        void BindQueue(const std::string &routingKey, std::shared_ptr<Queue> queue);
        void UnbindQueue(const std::string &routingKey, std::shared_ptr<Queue> queue);
        void RouteMessage(const std::string &routingKey, const Message &message);
        
        std::shared_ptr<Queue> GetQueue(const std::string &routingKey);
        inline size_t GetRoutingTableSize() const { return m_routingTable.size(); }
    };
}

#endif

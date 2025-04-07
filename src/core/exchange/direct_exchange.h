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

        void BindQueue(std::shared_ptr<Queue> queue, const std::optional<std::string> &bindingKey) override;
        void UnbindQueue(std::shared_ptr<Queue> queue, const std::optional<std::string> &bindingKey) override;
        void RouteMessage(const Message &message, const std::optional<std::string> &routingKey) override;
        
        std::shared_ptr<Queue> GetQueue(const std::string &routingKey);
        inline size_t GetRoutingTableSize() const { return m_routingTable.size(); }
    };
}

#endif

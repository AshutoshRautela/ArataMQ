#ifndef AR_TOPIC_EXCHANGE_H
#define AR_TOPIC_EXCHANGE_H

#include "iexchange.h"
#include <unordered_map>
#include <unordered_set>

namespace armq
{
    class TopicExchange : public IExchange
    {
    private:
        std::unordered_map<std::string, std::unordered_set<std::shared_ptr<Queue>>> m_bindingTable;

    public:
        TopicExchange(const std::string &name);

        bool TryMatchPattern(const std::string& pattern, const std::string& routingKey);
        void BindQueue(const std::string &pattern, std::shared_ptr<Queue> queue);
        void UnbindQueue(const std::string &pattern, std::shared_ptr<Queue> queue);
        void RouteMessage(const std::string &routingKey, const Message &message);
    };
}
#endif

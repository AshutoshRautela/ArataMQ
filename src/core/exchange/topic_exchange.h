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

        void BindQueue(std::shared_ptr<Queue> queue, const std::optional<std::string> &pattern) override;
        void UnbindQueue(std::shared_ptr<Queue> queue, const std::optional<std::string> &pattern) override;
        void RouteMessage(const Message &message, const std::optional<std::string> &routingKey) override;

        std::unordered_set<std::shared_ptr<Queue>> GetQueues(const std::string &pattern);
        size_t GetQueueSize(const std::string &pattern);
    };
}
#endif

#include "topic_exchange.h"
#include "utils/utils.h"

namespace armq
{
    TopicExchange::TopicExchange(const std::string &name) : IExchange(name, ExchangeType::TOPIC) {}

    bool TopicExchange::TryMatchPattern(const std::string &pattern, const std::string &routingKey)
    {
        std::vector<std::string> patternWords = SplitWords(pattern, ".");
        std::vector<std::string> routingWords = SplitWords(routingKey, ".");

        // Special case: # at start
        if (patternWords[0] == "#")
        {
            // Match if routing key ends with the rest of the pattern
            std::vector<std::string> remainingPattern(patternWords.begin() + 1, patternWords.end());
            if (routingWords.size() < remainingPattern.size())
                return false;
            return std::equal(remainingPattern.rbegin(), remainingPattern.rend(), routingWords.rbegin());
        }

        // Special case: # at end
        if (patternWords.back() == "#")
        {
            // Match if routing key starts with the pattern before #
            std::vector<std::string> prefixPattern(patternWords.begin(), patternWords.end() - 1);
            if (routingWords.size() < prefixPattern.size())
                return false;
            return std::equal(prefixPattern.begin(), prefixPattern.end(), routingWords.begin());
        }

        // Normal case: no #, just * and words
        if (patternWords.size() != routingWords.size())
            return false;

        for (size_t i = 0; i < patternWords.size(); ++i)
        {
            if (patternWords[i] == "*")
                continue; // * matches any word
            if (patternWords[i] != routingWords[i])
                return false;
        }

        return true;
    }

    void TopicExchange::BindQueue(std::shared_ptr<Queue> queue, const std::optional<std::string> &pattern)
    {
        if (!pattern.has_value())
            throw std::invalid_argument("Routing pattern is required");
        try
        {
            ValidateRoutingPattern(pattern.value());
            this->m_bindingTable[pattern.value()].insert(queue);
        }
        catch (const std::invalid_argument &e)
        {
            throw std::invalid_argument("Invalid routing pattern: " + pattern.value() + ". " + e.what());
        }
    }

    void TopicExchange::UnbindQueue(std::shared_ptr<Queue> queue, const std::optional<std::string> &pattern)
    {
        if (!pattern.has_value())
            throw std::invalid_argument("Routing pattern is required");
        try
        {
            ValidateRoutingPattern(pattern.value());
            if (this->m_bindingTable.count(pattern.value()) == 0)
                throw std::invalid_argument("No binding found for pattern: " + pattern.value());

            if (this->m_bindingTable[pattern.value()].count(queue) == 0)
                throw std::invalid_argument("Queue not found in binding for pattern: " + pattern.value());
        
            this->m_bindingTable[pattern.value()].erase(queue);
            if (this->m_bindingTable[pattern.value()].empty())
                this->m_bindingTable.erase(pattern.value());
        }
        catch (const std::invalid_argument &e)
        {
            throw std::invalid_argument("Invalid routing pattern: " + pattern.value() + ". " + e.what());
        }
    }

    void TopicExchange::RouteMessage(const Message &message, const std::optional<std::string> &routingKey)
    {
        if (!routingKey.has_value())
            throw std::invalid_argument("Routing key is required");
        try
        {
            bool messageRouted = false;
            ValidateRoutingKey(routingKey.value());
            for (const auto &[pattern, queues] : this->m_bindingTable)
            {
                if (this->TryMatchPattern(pattern, routingKey.value()))
                {
                    messageRouted = true;
                    for (const auto &queue : queues)
                    {
                        queue->Enqueue(message);
                    }
                }
            }

            if (!messageRouted)
            {
                throw std::invalid_argument("No matching pattern found for routing key: " + routingKey.value());
            }
        }
        catch (const std::invalid_argument &e)
        {
            throw std::invalid_argument("Error routing message: " + std::string(e.what()));
        }
    }

    size_t TopicExchange::GetQueueSize(const std::string &pattern)
    {
        try
        {
            if (this->m_bindingTable.find(pattern) == this->m_bindingTable.end())
                throw std::invalid_argument("No binding found for pattern: " + pattern);
            ValidateRoutingPattern(pattern);
            return this->m_bindingTable[pattern].size();
        }
        catch (const std::invalid_argument &e)
        {
            throw std::invalid_argument("Invalid routing pattern: " + pattern + ". " + e.what());
        }
    }

    std::unordered_set<std::shared_ptr<Queue>> TopicExchange::GetQueues(const std::string &pattern)
    {
        try
        {
            if (this->m_bindingTable.find(pattern) == this->m_bindingTable.end())
                throw std::invalid_argument("No binding found for pattern: " + pattern);
            ValidateRoutingPattern(pattern);
            return this->m_bindingTable[pattern];   
        }
        catch (const std::invalid_argument &e)
        {
            throw std::invalid_argument("Invalid routing pattern: " + pattern + ". " + e.what());
        }
    }
}

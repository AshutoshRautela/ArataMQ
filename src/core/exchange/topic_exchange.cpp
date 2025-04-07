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

    void TopicExchange::BindQueue(const std::string &pattern, std::shared_ptr<Queue> queue)
    {
        try
        {
            ValidateRoutingPattern(pattern);
            this->m_bindingTable[pattern].insert(queue);
        }
        catch (const std::invalid_argument &e)
        {
            throw std::invalid_argument("Invalid routing pattern: " + pattern + ". " + e.what());
        }
    }

    void TopicExchange::UnbindQueue(const std::string &pattern, std::shared_ptr<Queue> queue)
    {
        try
        {
            ValidateRoutingPattern(pattern);
            if (this->m_bindingTable.count(pattern) == 0)
                throw std::invalid_argument("No binding found for pattern: " + pattern);

            if (this->m_bindingTable[pattern].count(queue) == 0)
                throw std::invalid_argument("Queue not found in binding for pattern: " + pattern);
        
            this->m_bindingTable[pattern].erase(queue);
            if (this->m_bindingTable[pattern].empty())
                this->m_bindingTable.erase(pattern);
        }
        catch (const std::invalid_argument &e)
        {
            throw std::invalid_argument("Invalid routing pattern: " + pattern + ". " + e.what());
        }
    }

    void TopicExchange::RouteMessage(const std::string &routingKey, const Message &message)
    {
        try
        {
            bool messageRouted = false;
            ValidateRoutingKey(routingKey);
            for (const auto &[pattern, queues] : this->m_bindingTable)
            {
                if (this->TryMatchPattern(pattern, routingKey))
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
                throw std::invalid_argument("No matching pattern found for routing key: " + routingKey);
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

    std::unordered_set<std::shared_ptr<Queue>> TopicExchange::GetQueue(const std::string &pattern)
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

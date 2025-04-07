#include "broker.h"
#include "core/exchange/direct_exchange.h"
#include "core/exchange/topic_exchange.h"
#include "core/exchange/fanout_exchange.h"

namespace armq {
    Broker::Broker(): m_exchanges(std::unordered_map<std::string, std::shared_ptr<IExchange>>()) {
    }

    Broker::~Broker() {
    }

    std::shared_ptr<IExchange> Broker::CreateExchange(const std::string& name, const ExchangeType& type) {
        if (m_exchanges.count(name) > 0) 
            throw std::invalid_argument("Exchange already exists");

        if (type == ExchangeType::DIRECT) 
            m_exchanges[name] = std::make_shared<DirectExchange>(name);
        else if (type == ExchangeType::TOPIC) 
            m_exchanges[name] = std::make_shared<TopicExchange>(name);
        else if (type == ExchangeType::FANOUT) 
            m_exchanges[name] = std::make_shared<FanoutExchange>(name);
        else 
            throw std::invalid_argument("Invalid exchange type");
    
        return m_exchanges[name];
    }

    void Broker::DeleteExchange(const std::string& name) {
        if (m_exchanges.count(name) == 0) {
            throw std::invalid_argument("Exchange does not exist");
        }
        m_exchanges.erase(name);
    }   

    std::shared_ptr<IExchange> Broker::GetExchange(const std::string& name) {
        if (m_exchanges.count(name) == 0) {
            throw std::invalid_argument("Exchange does not exist");
        }
        return m_exchanges[name];
    }

    void Broker::BindQueue(const std::string& exchangeName, std::shared_ptr<Queue> queue, const std::optional<std::string>& bindingKey) {
        if (m_exchanges.count(exchangeName) == 0) {
            throw std::invalid_argument("Exchange does not exist");
        }
        m_exchanges[exchangeName]->BindQueue(queue, bindingKey);
    }

    void Broker::UnbindQueue(const std::string& exchangeName, std::shared_ptr<Queue> queue, const std::optional<std::string>& bindingKey) {
        if (m_exchanges.count(exchangeName) == 0) {
            throw std::invalid_argument("Exchange does not exist");
        }
        m_exchanges[exchangeName]->UnbindQueue(queue, bindingKey);
    }

    void Broker::Publish(const std::string& exchangeName, const std::string& routingKey, const Message& message) {
        if (m_exchanges.count(exchangeName) == 0) {
            throw std::invalid_argument("Exchange does not exist");
        }
        m_exchanges[exchangeName]->RouteMessage(message, routingKey);
    }
}
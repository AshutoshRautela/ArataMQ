#ifndef AR_BROKER_H
#define AR_BROKER_H

#include "core/exchange/iexchange.h"

namespace armq {
    class Broker {
        private:
            std::unordered_map<std::string, std::shared_ptr<IExchange>> m_exchanges;

        public:
            Broker();
            ~Broker();

            std::shared_ptr<IExchange> CreateExchange(const std::string& name, const ExchangeType& type);
            std::shared_ptr<IExchange> GetExchange(const std::string& name);

            void BindQueue(const std::string& exchangeName, std::shared_ptr<Queue> queue, const std::optional<std::string>& bindingKey = std::nullopt);
            void UnbindQueue(const std::string& exchangeName, std::shared_ptr<Queue> queue, const std::optional<std::string>& bindingKey = std::nullopt);

            void Publish(const std::string& exchangeName, const std::string& routingKey, const Message& message);

            void DeleteExchange(const std::string& name);
    };
}

#endif

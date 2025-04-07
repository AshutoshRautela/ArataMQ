#include <gtest/gtest.h>
#include "core/broker/broker.h"
#include "core/queue/queue.h"
#include "core/exchange/direct_exchange.h"
#include "core/exchange/topic_exchange.h"
#include "core/exchange/fanout_exchange.h"

namespace armq {
    namespace test {
        class BrokerTest: public ::testing::Test {
            protected:
                BrokerTest() {
                    broker = std::make_shared<Broker>();
                }

                std::shared_ptr<Broker> broker;
        };

        TEST_F(BrokerTest, CreateExchange) {;
            EXPECT_EQ(broker->CreateExchange("test_exchange", ExchangeType::DIRECT)->GetType(), ExchangeType::DIRECT);
            EXPECT_EQ(broker->CreateExchange("topic_exchange", ExchangeType::TOPIC)->GetType(), ExchangeType::TOPIC);

            std::shared_ptr<IExchange> fanout_exchange = broker->CreateExchange("fanout_exchange", ExchangeType::FANOUT);
            EXPECT_EQ(fanout_exchange->GetType(), ExchangeType::FANOUT);
            EXPECT_EQ(broker->GetExchange("fanout_exchange"), fanout_exchange);
        }

        TEST_F(BrokerTest, DeleteExchange) {
            broker->CreateExchange("test_exchange", ExchangeType::DIRECT);
            broker->DeleteExchange("test_exchange");

            EXPECT_THROW(broker->GetExchange("test_exchange"), std::invalid_argument);
            EXPECT_THROW(broker->DeleteExchange("test_exchange"), std::invalid_argument);
        }

        TEST_F(BrokerTest, BindQueue) {
            std::shared_ptr<Queue> directTestQueue = std::make_shared<Queue>("directTestQueue");
            std::shared_ptr<DirectExchange> direct_exchange = std::dynamic_pointer_cast<DirectExchange>(broker->CreateExchange("directExchange", ExchangeType::DIRECT));
            broker->BindQueue("directExchange", directTestQueue, "test.1");

            EXPECT_EQ(broker->GetExchange("directExchange"), direct_exchange);
            EXPECT_EQ(direct_exchange->GetQueue("test.1"), directTestQueue);

            std::shared_ptr<Queue> topicTestQueue = std::make_shared<Queue>("topicTestQueue");
            std::shared_ptr<TopicExchange> topic_exchange = std::dynamic_pointer_cast<TopicExchange>(broker->CreateExchange("topicExchange", ExchangeType::TOPIC));
            broker->BindQueue("topicExchange", topicTestQueue, "test.*");

            EXPECT_EQ(broker->GetExchange("topicExchange"), topic_exchange);
            EXPECT_EQ(topic_exchange->GetType(), ExchangeType::TOPIC);
            EXPECT_EQ(topic_exchange->GetQueues("test.*").size(), 1);

            std::shared_ptr<Queue> fanoutTestQueue = std::make_shared<Queue>("fanoutTestQueue");
            std::shared_ptr<FanoutExchange> fanout_exchange = std::dynamic_pointer_cast<FanoutExchange>(broker->CreateExchange("fanoutExchange", ExchangeType::FANOUT));
            broker->BindQueue("fanoutExchange", fanoutTestQueue);

            EXPECT_EQ(broker->GetExchange("fanoutExchange"), fanout_exchange);
            EXPECT_EQ(fanout_exchange->GetQueueCount(), 1);
        }
    }
}
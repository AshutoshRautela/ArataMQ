#include <gtest/gtest.h>
#include <memory>
#include "core/exchange/topic_exchange.h"

namespace armq {
    namespace test {
        class TopicExchangeTest: public::testing::Test {
            protected:
                TopicExchangeTest() {}

                // SetUp is called before each test
                // Initializes a new TopicExchange instance for testing
                void SetUp() override {
                    _topicExchange = std::make_shared<TopicExchange>("test_topic_exchange");
                }

                // TearDown is called after each test
                // Cleans up the TopicExchange instance
                void TearDown() override {

                }

                std::shared_ptr<TopicExchange> _topicExchange;
        };

        TEST_F(TopicExchangeTest, ValidateName) {
            EXPECT_EQ(_topicExchange->getName(), "test_topic_exchange");
        }

        TEST_F(TopicExchangeTest, TryMatchPattern) {
            EXPECT_TRUE(_topicExchange->TryMatchPattern("test.*", "test.1"));
            EXPECT_FALSE(_topicExchange->TryMatchPattern("test.*", "test.1.2"));
            EXPECT_FALSE(_topicExchange->TryMatchPattern("test.*", "test1"));
            EXPECT_TRUE(_topicExchange->TryMatchPattern("test.#", "test.1"));
            EXPECT_TRUE(_topicExchange->TryMatchPattern("test.#", "test.1.2"));
            EXPECT_FALSE(_topicExchange->TryMatchPattern("test.#", "test1"));
            EXPECT_TRUE(_topicExchange->TryMatchPattern("#", "test.1.2"));
            EXPECT_TRUE(_topicExchange->TryMatchPattern("*.*.*", "test.1.2"));
        } 
    }   
}
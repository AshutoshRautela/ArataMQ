#include <gtest/gtest.h>
#include <memory>
#include "core/exchange/topic_exchange.h"

namespace armq {
    namespace test {
        class TopicExchangeTest: public::testing::Test {
            protected:
                TopicExchangeTest(): _testMessage1(
                        Metadata{
                            "testMessageId_1",
                            "2021-01-01 12:00:00",
                            "testSource",
                            "testDestination",
                            "testType"
                        },
                        Headers{
                            "application/json",
                            "utf-8",
                            {
                                {"headerKey1", "headerValue1"},
                                {"headerKey2", "headerValue2"}
                            }
                        },
                        nlohmann::json({
                            {"key1", "value1"},
                            {"key2", "value2"}
                        })
                    ), _testMessage2(
                        Metadata{
                            "testMessageId_2",
                            "2021-01-01 12:00:00",
                            "testSource",
                            "testDestination",
                            "testType"
                        },
                        Headers{
                            "application/json",
                            "utf-8",
                            {
                                {"headerKey1", "headerValue1"},
                                {"headerKey2", "headerValue2"}
                            }
                        },
                        nlohmann::json({
                            {"key1", "value1"},
                            {"key2", "value2"}
                        })
                    )
                 {}

                // SetUp is called before each test
                // Initializes a new TopicExchange instance for testing
                void SetUp() override {
                    _topicExchange = std::make_shared<TopicExchange>("test_topic_exchange");
                    _testQueue1 = std::make_shared<Queue>("test_queue_1");
                    _testQueue2 = std::make_shared<Queue>("test_queue_2");
                    _testQueue3 = std::make_shared<Queue>("test_queue_3");
                    _testQueue4 = std::make_shared<Queue>("test_queue_4");
                }

                // TearDown is called after each test
                // Cleans up the TopicExchange instance
                void TearDown() override {
                    _topicExchange.reset();
                }

                std::shared_ptr<TopicExchange> _topicExchange;
                std::shared_ptr<Queue> _testQueue1;
                std::shared_ptr<Queue> _testQueue2;
                std::shared_ptr<Queue> _testQueue3;
                std::shared_ptr<Queue> _testQueue4;
                Message _testMessage1;
                Message _testMessage2;
        };

        TEST_F(TopicExchangeTest, ValidateName) {
            EXPECT_EQ(_topicExchange->GetName(), "test_topic_exchange");
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

        TEST_F(TopicExchangeTest, BindQueue) {
            _topicExchange->BindQueue(_testQueue1, "test.*");
            _topicExchange->BindQueue(_testQueue2, "test.1");
            
            EXPECT_EQ(_topicExchange->GetQueues("test.*").count(_testQueue1), true);
            EXPECT_EQ(_topicExchange->GetQueueSize("test.1"), 1);
            EXPECT_EQ(_topicExchange->GetQueueSize("test.*"), 1);

            _topicExchange->BindQueue(_testQueue2, "test.*");
            EXPECT_EQ(_topicExchange->GetQueues("test.*").count(_testQueue2), true);
            EXPECT_EQ(_topicExchange->GetQueueSize("test.*"), 2);

            _topicExchange->BindQueue(_testQueue3, "test.1");
            EXPECT_EQ(_topicExchange->GetQueues("test.1").count(_testQueue3), true);
            EXPECT_EQ(_topicExchange->GetQueueSize("test.1"), 2);
        }

        TEST_F(TopicExchangeTest, UnbindQueue) {
            _topicExchange->BindQueue(_testQueue1, "test.*");
            EXPECT_EQ(_topicExchange->GetQueueSize("test.*"), 1);
            _topicExchange->BindQueue(_testQueue2, "test.1");
            EXPECT_EQ(_topicExchange->GetQueueSize("test.1"), 1);

            _topicExchange->BindQueue(_testQueue2, "test.*");
            EXPECT_EQ(_topicExchange->GetQueueSize("test.*"), 2);

            _topicExchange->UnbindQueue(_testQueue1, "test.*");
            EXPECT_EQ(_topicExchange->GetQueues("test.*").count(_testQueue1), false);
            EXPECT_EQ(_topicExchange->GetQueueSize("test.*"), 1);

            _topicExchange->UnbindQueue(_testQueue2, "test.1");
            EXPECT_THROW(_topicExchange->GetQueues("test.1"), std::invalid_argument);
            EXPECT_THROW(_topicExchange->GetQueueSize("test.1"), std::invalid_argument);
            
            _topicExchange->UnbindQueue(_testQueue2, "test.*");
            EXPECT_THROW(_topicExchange->GetQueues("test.*"), std::invalid_argument);
            EXPECT_THROW(_topicExchange->GetQueueSize("test.*"), std::invalid_argument);

            EXPECT_THROW(_topicExchange->UnbindQueue(_testQueue2, "test.*"), std::invalid_argument);
            EXPECT_THROW(_topicExchange->UnbindQueue(_testQueue1, "test.1"), std::invalid_argument);
        }
        
        TEST_F(TopicExchangeTest, RouteMessage) {
            _topicExchange->BindQueue(_testQueue1, "test.*");
            _topicExchange->BindQueue(_testQueue2, "test.1");
            _topicExchange->BindQueue(_testQueue3, "test.#");

            _topicExchange->RouteMessage(_testMessage1, "test.1");

            EXPECT_EQ(_testQueue1->Size(), 1);
            EXPECT_EQ(_testQueue2->Size(), 1);
            EXPECT_EQ(_testQueue3->Size(), 1);

            _topicExchange->RouteMessage(_testMessage2, "test.2");
            EXPECT_EQ(_testQueue1->Size(), 2);
            EXPECT_EQ(_testQueue2->Size(), 1);
            EXPECT_EQ(_testQueue3->Size(), 2);

            _topicExchange->RouteMessage(_testMessage2, "test.3.1");
            EXPECT_EQ(_testQueue1->Size(), 2);
            EXPECT_EQ(_testQueue2->Size(), 1);
            EXPECT_EQ(_testQueue3->Size(), 3);
        }
        
    }   
}
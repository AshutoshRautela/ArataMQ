#include <gtest/gtest.h>
#include <memory>
#include "core/exchange/direct_exchange.h"
#include "core/queue/queue.h"

namespace armq {
    namespace test {
        class DirectExchangeTest : public ::testing::Test {
            protected:
                DirectExchangeTest(): _testMessage1(
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
                void SetUp() override {
                    _directExchange = std::make_shared<DirectExchange>("test_direct_exchange");
                    _testQueue1 = std::make_shared<Queue>("test_queue_1");
                    _testQueue2 = std::make_shared<Queue>("test_queue_2");
                    _testQueue3 = std::make_shared<Queue>("test_queue_3");
                    _testQueue4 = std::make_shared<Queue>("test_queue_4");
                }

                // TearDown is called after each test
                void TearDown() override {
                    _directExchange.reset();
                    _testQueue1.reset();
                    _testQueue2.reset();
                    _testQueue3.reset();
                    _testQueue4.reset();
                }

                std::shared_ptr<DirectExchange> _directExchange;
                std::shared_ptr<armq::Queue> _testQueue1;
                std::shared_ptr<armq::Queue> _testQueue2;
                std::shared_ptr<armq::Queue> _testQueue3;
                std::shared_ptr<armq::Queue> _testQueue4;

                Message _testMessage1;
                Message _testMessage2;
        };

        TEST_F(DirectExchangeTest, ValidateName) {
            EXPECT_EQ(_directExchange->GetName(), "test_direct_exchange");
        }

        // Comment out these tests until the DirectExchange methods are implemented
        
        TEST_F(DirectExchangeTest, BindQueue) {
            _directExchange->BindQueue(_testQueue1, "routing_key_1");
            EXPECT_EQ(_directExchange->GetQueue("routing_key_1"), _testQueue1);
            EXPECT_EQ(_directExchange->GetRoutingTableSize(), 1);

            _directExchange->BindQueue(_testQueue2, "routing_key_2");
            EXPECT_EQ(_directExchange->GetQueue("routing_key_2"), _testQueue2);
            EXPECT_EQ(_directExchange->GetRoutingTableSize(), 2);

            _directExchange->BindQueue(_testQueue3, "routing_key_3");
            EXPECT_EQ(_directExchange->GetQueue("routing_key_3"), _testQueue3);
            EXPECT_EQ(_directExchange->GetRoutingTableSize(), 3);

            _directExchange->BindQueue(_testQueue4, "routing_key_4");
            EXPECT_EQ(_directExchange->GetQueue("routing_key_4"), _testQueue4);
            EXPECT_EQ(_directExchange->GetRoutingTableSize(), 4);
        }

        TEST_F(DirectExchangeTest, UnbindQueue) {
            _directExchange->BindQueue(_testQueue1, "routing_key_1");
            _directExchange->BindQueue(_testQueue2, "routing_key_2");
            _directExchange->BindQueue(_testQueue3, "routing_key_3");
            _directExchange->BindQueue(_testQueue4, "routing_key_4");
            EXPECT_EQ(_directExchange->GetRoutingTableSize(), 4);

            EXPECT_EQ(_directExchange->GetQueue("routing_key_1"), _testQueue1);
            _directExchange->UnbindQueue(_testQueue1, "routing_key_1");
            EXPECT_THROW(_directExchange->GetQueue("routing_key_1"), std::runtime_error);
            EXPECT_EQ(_directExchange->GetRoutingTableSize(), 3);

            EXPECT_EQ(_directExchange->GetQueue("routing_key_2"), _testQueue2);
            _directExchange->UnbindQueue(_testQueue2, "routing_key_2");
            EXPECT_THROW(_directExchange->GetQueue("routing_key_2"), std::runtime_error);
            EXPECT_EQ(_directExchange->GetRoutingTableSize(), 2);

            EXPECT_EQ(_directExchange->GetQueue("routing_key_3"), _testQueue3);
            _directExchange->UnbindQueue(_testQueue3, "routing_key_3");
            EXPECT_THROW(_directExchange->GetQueue("routing_key_3"), std::runtime_error);
            EXPECT_EQ(_directExchange->GetRoutingTableSize(), 1);

            EXPECT_EQ(_directExchange->GetQueue("routing_key_4"), _testQueue4);
            _directExchange->UnbindQueue(_testQueue4, "routing_key_4");
            EXPECT_THROW(_directExchange->GetQueue("routing_key_4"), std::runtime_error);
            EXPECT_EQ(_directExchange->GetRoutingTableSize(), 0);
        }

        TEST_F(DirectExchangeTest, RouteMessage) {
            _directExchange->BindQueue(_testQueue1, "routing_key_1");
            _directExchange->BindQueue(_testQueue2, "routing_key_2");

            EXPECT_EQ(_directExchange->GetQueue("routing_key_1"), _testQueue1);
            _directExchange->RouteMessage(_testMessage1, "routing_key_1");
            EXPECT_EQ(_testQueue1->Size(), 1);
            EXPECT_EQ(_testQueue1->Dequeue(), _testMessage1);

            EXPECT_EQ(_directExchange->GetQueue("routing_key_2"), _testQueue2);
            _directExchange->RouteMessage(_testMessage1, "routing_key_2");
            _directExchange->RouteMessage(_testMessage2, "routing_key_2");
            EXPECT_EQ(_testQueue2->Size(), 2);
            EXPECT_EQ(_testQueue2->Dequeue(), _testMessage1);
            EXPECT_EQ(_testQueue2->Size(), 1);
            EXPECT_EQ(_testQueue2->Dequeue(), _testMessage2);
        }
    }  
}
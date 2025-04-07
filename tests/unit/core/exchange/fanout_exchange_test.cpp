#include <gtest/gtest.h>
#include <memory>
#include "core/exchange/fanout_exchange.h"

namespace armq {
    namespace test {
        class FanoutExchangeTest : public ::testing::Test { 
            protected:
                FanoutExchangeTest(): _testMessage1(
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
                    _fanoutExchange = std::make_shared<FanoutExchange>("test_fanout_exchange");
                    _testQueue1 = std::make_shared<Queue>("test_queue_1");
                    _testQueue2 = std::make_shared<Queue>("test_queue_2");
                    _testQueue3 = std::make_shared<Queue>("test_queue_3");
                    _testQueue4 = std::make_shared<Queue>("test_queue_4");
                }   

                void TearDown() override {
                    _fanoutExchange.reset();
                }

                std::shared_ptr<FanoutExchange> _fanoutExchange;
                std::shared_ptr<Queue> _testQueue1;
                std::shared_ptr<Queue> _testQueue2;
                std::shared_ptr<Queue> _testQueue3;
                std::shared_ptr<Queue> _testQueue4;

                Message _testMessage1;
                Message _testMessage2;
        };

        TEST_F(FanoutExchangeTest, ValidateName) {
            EXPECT_EQ(_fanoutExchange->GetName(), "test_fanout_exchange");
        }

        TEST_F(FanoutExchangeTest, BindQueue) {
            _fanoutExchange->BindQueue(_testQueue1);
            EXPECT_EQ(_fanoutExchange->GetQueueCount(), 1);

            _fanoutExchange->BindQueue(_testQueue2);
            EXPECT_EQ(_fanoutExchange->GetQueueCount(), 2);

            _fanoutExchange->BindQueue(_testQueue3);
            EXPECT_EQ(_fanoutExchange->GetQueueCount(), 3);

            _fanoutExchange->BindQueue(_testQueue4);
            EXPECT_EQ(_fanoutExchange->GetQueueCount(), 4);

            _fanoutExchange->BindQueue(_testQueue1);
            EXPECT_EQ(_fanoutExchange->GetQueueCount(), 4);

            _fanoutExchange->BindQueue(_testQueue2);
            EXPECT_EQ(_fanoutExchange->GetQueueCount(), 4);

            _fanoutExchange->BindQueue(_testQueue2);
            EXPECT_EQ(_fanoutExchange->GetQueueCount(), 4);

            _fanoutExchange->BindQueue(_testQueue3);
            EXPECT_EQ(_fanoutExchange->GetQueueCount(), 4);
        }

        TEST_F(FanoutExchangeTest, UnbindQueue) {
            EXPECT_THROW(_fanoutExchange->UnbindQueue(_testQueue1), std::runtime_error);

            _fanoutExchange->BindQueue(_testQueue1);
            _fanoutExchange->BindQueue(_testQueue2);
            _fanoutExchange->BindQueue(_testQueue3);
            _fanoutExchange->BindQueue(_testQueue4);

            EXPECT_EQ(_fanoutExchange->GetQueueCount(), 4);

            _fanoutExchange->UnbindQueue(_testQueue1);
            EXPECT_EQ(_fanoutExchange->GetQueueCount(), 3);

            _fanoutExchange->UnbindQueue(_testQueue2);
            EXPECT_EQ(_fanoutExchange->GetQueueCount(), 2);

            _fanoutExchange->UnbindQueue(_testQueue3);
            EXPECT_EQ(_fanoutExchange->GetQueueCount(), 1);

            _fanoutExchange->UnbindQueue(_testQueue4);
            EXPECT_EQ(_fanoutExchange->GetQueueCount(), 0);
            
            EXPECT_THROW(_fanoutExchange->UnbindQueue(_testQueue1), std::runtime_error);
        }

        TEST_F(FanoutExchangeTest, RouteMessage) {
            _fanoutExchange->BindQueue(_testQueue1);
            _fanoutExchange->BindQueue(_testQueue2);
            EXPECT_EQ(_fanoutExchange->GetQueueCount(), 2);

            _fanoutExchange->BindQueue(_testQueue3);
            _fanoutExchange->BindQueue(_testQueue4);
            EXPECT_EQ(_fanoutExchange->GetQueueCount(), 4);

            _fanoutExchange->RouteMessage(_testMessage1);
            EXPECT_EQ(_testQueue1->Size(), 1);
            EXPECT_EQ(_testQueue1->Dequeue(), _testMessage1);

            _fanoutExchange->RouteMessage(_testMessage1);
            _fanoutExchange->RouteMessage(_testMessage2);
            EXPECT_EQ(_testQueue2->Size(), 3);
            EXPECT_EQ(_testQueue2->Dequeue(), _testMessage1);
            EXPECT_EQ(_testQueue2->Size(), 2);
            EXPECT_EQ(_testQueue2->Dequeue(), _testMessage1);
            EXPECT_EQ(_testQueue2->Size(), 1);
            EXPECT_EQ(_testQueue2->Dequeue(), _testMessage2);
        }
    }   
}
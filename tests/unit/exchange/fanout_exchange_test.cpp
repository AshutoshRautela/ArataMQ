#include <gtest/gtest.h>
#include <memory>
#include "core/exchange/fanout_exchange.h"

namespace armq {
    namespace test {
        class FanoutExchangeTest : public ::testing::Test { 
            protected:
                FanoutExchangeTest() {}

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
        };

        TEST_F(FanoutExchangeTest, ValidateName) {
            EXPECT_EQ(_fanoutExchange->getName(), "test_fanout_exchange");
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
    }   
}
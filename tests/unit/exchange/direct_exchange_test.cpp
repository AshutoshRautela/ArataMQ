#include <gtest/gtest.h>
#include <memory>
#include "core/exchange/direct_exchange.h"
#include "core/queue/queue.h"

namespace armq {
    namespace test {
        class DirectExchangeTest : public ::testing::Test {
            protected:
                DirectExchangeTest() {}

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
        };

        TEST_F(DirectExchangeTest, ValidateName) {
            EXPECT_EQ(_directExchange->getName(), "test_direct_exchange");
        }

        // Comment out these tests until the DirectExchange methods are implemented
        
        TEST_F(DirectExchangeTest, BindQueue) {
            _directExchange->BindQueue("routing_key_1", _testQueue1);
            EXPECT_EQ(_directExchange->GetQueue("routing_key_1"), _testQueue1);
            EXPECT_EQ(_directExchange->GetRoutingTableSize(), 1);

            _directExchange->BindQueue("routing_key_2", _testQueue2);
            EXPECT_EQ(_directExchange->GetQueue("routing_key_2"), _testQueue2);
            EXPECT_EQ(_directExchange->GetRoutingTableSize(), 2);

            _directExchange->BindQueue("routing_key_3", _testQueue3);
            EXPECT_EQ(_directExchange->GetQueue("routing_key_3"), _testQueue3);
            EXPECT_EQ(_directExchange->GetRoutingTableSize(), 3);

            _directExchange->BindQueue("routing_key_4", _testQueue4);
            EXPECT_EQ(_directExchange->GetQueue("routing_key_4"), _testQueue4);
            EXPECT_EQ(_directExchange->GetRoutingTableSize(), 4);
        }

        TEST_F(DirectExchangeTest, UnbindQueue) {
            _directExchange->BindQueue("routing_key_1", _testQueue1);
            _directExchange->BindQueue("routing_key_2", _testQueue2);
            _directExchange->BindQueue("routing_key_3", _testQueue3);
            _directExchange->BindQueue("routing_key_4", _testQueue4);
            EXPECT_EQ(_directExchange->GetRoutingTableSize(), 4);

            EXPECT_EQ(_directExchange->GetQueue("routing_key_1"), _testQueue1);
            _directExchange->UnbindQueue("routing_key_1", _testQueue1);
            EXPECT_THROW(_directExchange->GetQueue("routing_key_1"), std::runtime_error);
            EXPECT_EQ(_directExchange->GetRoutingTableSize(), 3);

            EXPECT_EQ(_directExchange->GetQueue("routing_key_2"), _testQueue2);
            _directExchange->UnbindQueue("routing_key_2", _testQueue2);
            EXPECT_THROW(_directExchange->GetQueue("routing_key_2"), std::runtime_error);
            EXPECT_EQ(_directExchange->GetRoutingTableSize(), 2);

            EXPECT_EQ(_directExchange->GetQueue("routing_key_3"), _testQueue3);
            _directExchange->UnbindQueue("routing_key_3", _testQueue3);
            EXPECT_THROW(_directExchange->GetQueue("routing_key_3"), std::runtime_error);
            EXPECT_EQ(_directExchange->GetRoutingTableSize(), 1);

            EXPECT_EQ(_directExchange->GetQueue("routing_key_4"), _testQueue4);
            _directExchange->UnbindQueue("routing_key_4", _testQueue4);
            EXPECT_THROW(_directExchange->GetQueue("routing_key_4"), std::runtime_error);
            EXPECT_EQ(_directExchange->GetRoutingTableSize(), 0);
        }
    }  
}
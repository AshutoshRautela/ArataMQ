#include "gtest/gtest.h"
#include "core/exchange/direct_exchange.h"

namespace armq {
    namespace test {
        class DirectExchangeTest : public ::testing::Test {
            protected:
                void SetUp() override {
                    _directExchange = std::make_shared<DirectExchange>("test_direct_exchange");
                }

                void TearDown() override {
                    _directExchange.reset();
                }

                std::shared_ptr<DirectExchange> _directExchange;
        };

        TEST_F(DirectExchangeTest, ValidateName) {
            EXPECT_EQ(_directExchange->getName(), "test_direct_exchange");
        }
    }  
}
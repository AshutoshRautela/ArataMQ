#include <gtest/gtest.h>
#include <memory>
#include <nlohmann/json.hpp>
#include "core/queue/queue.h"

namespace armq
{
    namespace test {
        class QueueTest: public ::testing::Test {
            protected:
                QueueTest() : 
                    testMessage1(
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
                    ),
                    testMessage2(
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

                void SetUp() override {
                    queue = std::make_shared<Queue>("test_queue");
                }

                void TearDown() override {
                    this->queue.reset();
                }

                Message testMessage1;
                Message testMessage2;
                std::shared_ptr<Queue> queue;
        };

        TEST_F(QueueTest, ValidateName) {
            EXPECT_EQ(queue->getName(), "test_queue");
        }

        TEST_F(QueueTest, Enqueue){
            EXPECT_EQ(queue->IsEmpty(), true);

            queue->Enqueue(testMessage1);
            EXPECT_EQ(queue->Size(), 1);

            queue->Enqueue(testMessage2);
            EXPECT_EQ(queue->Size(), 2);
        }

        TEST_F(QueueTest, Dequeue){
            EXPECT_THROW(queue->Dequeue(), std::runtime_error);

            queue->Enqueue(testMessage1);
            queue->Enqueue(testMessage2);
            EXPECT_EQ(queue->Size(), 2);

            Message message = queue->Dequeue();
            EXPECT_EQ(message, testMessage1);
            EXPECT_EQ(queue->Size(), 1);

            message = queue->Dequeue();
            EXPECT_EQ(message, testMessage2);
            EXPECT_EQ(queue->IsEmpty(), true);

            EXPECT_THROW(queue->Dequeue(), std::runtime_error);
        }
    }
}

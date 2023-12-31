#include <gtest/gtest.h>
#include "../sdk/message.h"

TEST(MessageTest, DefaultConstructor) {
    // Create a Message object using the default constructor
    Message message;

    // Perform assertions to verify the expected behavior
    // For example, you can check if the message object is not null
    ASSERT_NE(nullptr, &message);
}

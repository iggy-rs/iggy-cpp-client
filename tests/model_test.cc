#include <gtest/gtest.h>
#include "../sdk/model.h"

TEST(ModelTest, DefaultConstructor) {
    // Create a Message object using the default constructor
    iggy::model::system::Stats stats;

    // Perform assertions to verify the expected behavior
    // For example, you can check if the message object is not null
    ASSERT_NE(nullptr, &stats);
}

#include <gtest/gtest.h>
#include "../sdk/models.h"

TEST(ModelsTest, DefaultConstructor) {
    // Create a Message object using the default constructor
    iggy::models::Stats stats;

    // Perform assertions to verify the expected behavior
    // For example, you can check if the message object is not null
    ASSERT_NE(nullptr, &stats);
}

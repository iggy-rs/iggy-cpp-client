#define CATCH_CONFIG_MAIN
#include "unit_testutils.h"
#include "../sdk/model.h"

TEST_CASE("simple test for model objects", UT_TAG) {
    iggy::model::system::Stats stats;
    REQUIRE(&stats != nullptr);
}

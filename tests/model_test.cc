#include "../sdk/model.h"
#include "unit_testutils.h"

TEST_CASE("model objects", UT_TAG) {
    iggy::model::system::Stats stats;
    REQUIRE(&stats != nullptr);
}

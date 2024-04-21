#include "../sdk/model.h"
#include "unit_testutils.h"

TEST_CASE("model objects", UT_TAG) {
    icp::model::system::Stats stats;
    REQUIRE(&stats != nullptr);
}

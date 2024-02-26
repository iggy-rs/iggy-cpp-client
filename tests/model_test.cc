#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "../sdk/model.h"

TEST_CASE("simple test for model objects", "[model]") {
    iggy::model::system::Stats stats;
    REQUIRE(&stats != nullptr);
}

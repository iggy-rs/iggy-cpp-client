#include "../sdk/client.h"
#include "unit_testutils.h"

TEST_CASE("client connection", UT_TAG) {
    iggy::client::Options options;
    auto client = iggy::client::Client(options);
    auto stats = client.getStats();
    REQUIRE(&stats != nullptr);
}

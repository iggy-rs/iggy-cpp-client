#include <catch.hpp>
#include "e2e_testutils.h"
#include "../sdk/client.h"

TEST_CASE("E2E test for ping command", "[ping]") {
    // Start the Docker container; shuts down when this object goes out of scope
    IggyRunner runner;

    // Create a client object with all defaults
    iggy::client::Options options;
    iggy::client::Client client(options);

    // Perform the ping operation
    client.ping();
}

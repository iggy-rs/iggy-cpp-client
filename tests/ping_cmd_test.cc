#include "../sdk/client.h"
#include "e2e_testutils.h"

TEST_CASE("ping Iggy server", E2E_TAG) {
    // Start the Docker container; shuts down when this object goes out of scope
    IggyRunner runner;

    // Create a client object with all defaults
    iggy::client::Options options;
    iggy::client::Client client(options);

    // Perform the ping operation
    client.ping();
}

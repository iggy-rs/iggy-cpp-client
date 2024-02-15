#include "e2e_testutils.h"
#include "../sdk/client.h"

TEST_F(DockerTest, BinaryPing) {
    // Create a client object with all defaults
    iggy::client::Options options;
    iggy::client::Client client(options);

    // Perform the ping operation
    client.ping();
}

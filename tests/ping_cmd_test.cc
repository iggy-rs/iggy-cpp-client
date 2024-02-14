#include "e2e_testutils.h"

TEST_F(DockerTest, BinaryPing) {
    // temporary: using the Rust CLI from Docker to execute the ping to demonstrate test fixture
    reproc::process ping_process;
    reproc::options options;
    options.redirect.parent = true;
    std::vector<std::string> ping_args = {"docker", "exec", "iggy-server", "/iggy/iggy", "ping"};
    ping_process.start(ping_args, options);
    ping_process.wait(reproc::milliseconds(5000));
}

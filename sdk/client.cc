#include "client.h"
#include <reproc++/reproc.hpp>
#include <string>
#include <vector>

/* Loop wakeup handler */
void on_wakeup(struct us_loop_t* loop) {}

/* Loop pre iteration handler */
void on_pre(struct us_loop_t* loop) {}

/* Loop post iteration handler */
void on_post(struct us_loop_t* loop) {}

iggy::client::Client::Client(Options& options) {
    // to make more natural interface for setting options we use a struct, so need to validate it.
    options.validate();

    // does nothing for now, but will be used to set up the client connection with uSockets
    struct us_loop_t* loop = us_create_loop(0, on_wakeup, on_pre, on_post, 0);
    us_loop_run(loop);
    us_loop_free(loop);
}

void iggy::client::Client::ping() {
    // temporary! using the Rust CLI from Docker to execute the ping to demonstrate test fixture
    reproc::process ping_process;
    reproc::options options;
    options.redirect.parent = true;
    std::vector<std::string> ping_args = {"docker", "exec", "iggy_test", "/iggy/iggy", "ping"};
    ping_process.start(ping_args, options);
    ping_process.wait(reproc::milliseconds(5000));
}

iggy::model::system::Stats iggy::client::Client::getStats() {
    // temporary! need to send GetStats command to server then use WireFormat to read the response
    return iggy::model::system::Stats();
}

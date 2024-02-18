#pragma once

#include <sodium.h>
#include <stdexcept>
#include <string>
#include "model.h"

namespace iggy {
namespace transport {

/**
 * @enum Transport
 * @brief Available network transports for the Iggy server. Not all currently supported by the C++ client.
 */
enum Transport {
    /**
     * @brief Modern networking protocol from Google built on top of UDP.
     *
     * @ref [Wikipedia](https://en.wikipedia.org/wiki/QUIC)
     */
    QUIC,

    /**
     * @brief Classic HTTP REST encoded as JSON. Not recommended for high performance applications.
     */
    HTTP,

    /**
     * @brief Binary protocol over TCP/IP. This is the default transport.
     */
    TCP
};
};  // namespace transport
namespace client {

/**
 * @class Credentials
 * @brief Iggy server login credentials: username and password for now.
 *
 * This class provides a secure holder for the password; when destroyed, it zeroes of the memory.
 */
class Credentials {
private:
    std::string username;
    std::string password;

public:
    Credentials(const std::string& username, const std::string& password)
        : username(username)
        , password(password) {}

    ~Credentials() { sodium_memzero(&password[0], password.size()); }
};

const unsigned short DEFAULT_HTTP_PORT = 3000;
const unsigned short DEFAULT_TCP_PORT = 8090;
const unsigned short DEFAULT_QUIC_PORT = 8080;

/**
 * @struct Options
 * @brief A struct to hold various options.
 *
 * This struct is used to hold various options and settings
 * that are used throughout the application. Each field in the
 * struct represents a different option.
 */
struct Options {
    /**
     * @brief The hostname of the Iggy server.
     */
    std::string hostname = "localhost";

    /**
     * @brief The port the Iggy server is listening on; default depends on transport. Defaults to the DEFAULT_TCP_PORT.
     */
    unsigned short port = DEFAULT_TCP_PORT;

    /**
     * @brief The network transport to use when connecting to the server. Defaults to TCP.
     */
    iggy::transport::Transport transport = iggy::transport::Transport::TCP;

    /**
     * @brief The user credentials to use when connecting to the server.
     *
     * Note that the default credentials are built-in and are not secure. We recommend that you create a new
     * admin user with a strong password and then deactivate this user.
     */
    client::Credentials credentials = client::Credentials("iggy", "iggy");

    void validate() {
        if (hostname.empty()) {
            throw std::invalid_argument("Hostname cannot be empty");
        }
    }
};

/**
 * @class Client
 * @brief Flexible client for connecting to the Iggy server.
 *
 * Main Iggy C++ client. You can use Options to configure the endpoint, credentials and transport.
 */
class Client {
public:
    Client(Options& options);

    /**
     * @brief Send a synchronous ping to the server to check if it is alive.
     */
    void ping();

    /**
     * @brief Get the Iggy server's performance statistics.
     */
    iggy::model::Stats getStats();
};
};  // namespace client
};  // namespace iggy

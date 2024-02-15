#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace iggy {

/**
 * @namespace binary
 * @brief All related types for the binary format used in Iggy's QUIC transport.
 *
 * The definitions \htmlonly <a href="https://docs.iggy.rs/specification/binary">binary specification</a> \endhtmlonly
 * which should be taken as canonical; E2E conformance tests will be used to ensure client stays in line. I am not sure
 * about this design yet; ideally I want minimal duplication between the binary and JSON variations, and so may
 * introduce format-specific serializers. This is less natural in C++ than Rust.
 */
namespace binary {

/**
 * @enum CommandCode
 * @brief Complete list of supported integer codes for the Iggy protocol.
 */
enum CommandCode {
    PING = 1,
    GET_STATS = 10,
    GET_ME = 20,
    GET_CLIENT = 21,
    GET_CLIENTS = 22,
    POLL_MESSAGES = 100,
    SEND_MESSAGES = 101,
    GET_CONSUMER_OFFSET = 120,
    STORE_CONSUMER_OFFSET = 121,
    GET_STREAM = 200,
    GET_STREAMS = 201,
    CREATE_STREAM = 202,
    DELETE_STREAM = 203,
    GET_TOPIC = 300,
    GET_TOPICS = 301,
    CREATE_TOPIC = 302,
    DELETE_TOPIC = 303,
    CREATE_PARTITIONS = 402,
    DELETE_PARTITIONS = 403,
    GET_CONSUMER_GROUP = 600,
    GET_CONSUMER_GROUPS = 601,
    CREATE_CONSUMER_GROUP = 602,
    DELETE_CONSUMER_GROUP = 603,
    JOIN_CONSUMER_GROUP = 604,
    LEAVE_CONSUMER_GROUP = 605
};

/**
 * @class SerializedCommand
 * @brief Command header and payload for the Iggy binary protocol.
 */
class SerializedCommand {
private:
    uint32_t length;
    CommandCode command_code;
    std::vector<unsigned char> payload;

public:
    SerializedCommand() = default;
};

namespace shared {
enum ConsumerKind { CONSUMER = 1, CONSUMER_GROUP = 2 };

class Consumer {
private:
    ConsumerKind kind;
    uint32_t id;
public:
    Consumer(ConsumerKind kind, uint32_t id);
    ConsumerKind getKind();
    uint32_t getId();
};

enum IdKind { NUMERIC = 1, STRING = 2 };

class Identifier {
private:
    IdKind kind;
    uint8_t length;
    std::vector<unsigned char> value;
};

class PolledMessages {};

class Message {};

class ConsumerOffsetInfo {};
}  // namespace shared

namespace command {
class Command {
public:
    virtual CommandCode getCommandCode() = 0;
    virtual SerializedCommand serialize() = 0;
};

namespace stream {
class GetStream {};
class GetStreams {};
class CreateStream {};
class DeleteStream {};
}  // namespace stream

namespace topic {
class GetTopic {};
class GetTopics {};
class CreateTopic {};
}  // namespace topic

namespace partition {
class CreatePartitions {};
class DeletePartitions {};
}  // namespace partition

namespace message {
enum PollingKind { OFFSET = 1, TIMESTAMP = 2, FIRST = 3, LAST = 4, NEXT = 5 };

class PollingStrategy {};
class PolledMessages {};

enum PartitioningKind { BALANCED = 1, PARITION_ID = 2, MESSAGES_KEY = 3 };

typedef std::string HeaderKey;

enum HeaderKind {
    RAW = 1,
    STRING = 2,
    BOOL = 3,
    INT8 = 4,
    INT16 = 5,
    INT32 = 6,
    INT64 = 7,
    INT128 = 8,
    UINT8 = 9,
    UINT16 = 10,
    UINT32 = 11,
    UINT64 = 12,
    UINT128 = 13,
    FLOAT32 = 14,
    FLOAT64 = 15
};

class HeaderValue {};

class Message {};
class SendMessages : public Command {
public:
    CommandCode getCommandCode() override { return command_code; }

    static const CommandCode command_code = CommandCode::SEND_MESSAGES;
};

}  // namespace message

namespace consumeroffset {
class GetConsumerOffset {
public:
    static const CommandCode command_code = CommandCode::GET_CONSUMER_OFFSET;
};
class StoreConsumerOffset {
public:
    static const CommandCode command_code = CommandCode::GET_CONSUMER_OFFSET;
};
}  // namespace consumeroffset

namespace consumergroup {
class GetConsumerGroup {};
class GetConsumerGroups {};
class CreateConsumerGroup {};
class DeleteConsumerGroup {};
class JoinConsumerGroup {};
class LeaveConsumerGroup {};
}  // namespace consumergroup

namespace system {
class Ping {};
class GetMe {};
class GetClient {};
class GetClients {};
class GetStats {};
}  // namespace system
}  // namespace command
}  // namespace binary
}  // namespace iggy

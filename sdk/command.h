#pragma once

#include <string>
#include <vector>

namespace iggy {

/**
 * @namespace command
 * @brief All supported commands in the Iggy protocol.
 * 
 * Serialization-agnostic Command classes that you can send via an @ref iggy::client::Client.
 */
namespace command {

class Command {
public:
    virtual ~Command() = default;
};

/**
 * @namespace shared
 * @brief Shared types used across multiple Commands.
 */
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

/**
 * @namespace stream
 * @brief Commands related to managing Iggy streams.
 */
namespace stream {
class GetStream : Command {};
class GetStreams : Command {};
class CreateStream : Command {};
class DeleteStream : Command {};
}  // namespace stream

/**
 * @namespace topic
 * @brief Commands related to managing Iggy topics.
 */
namespace topic {
class GetTopic : Command {};
class GetTopics : Command {};
class CreateTopic : Command {};
}  // namespace topic

/**
 * @namespace partition
 * @brief Commands related to managing Iggy partitions.
 */
namespace partition {
class CreatePartitions : Command {};
class DeletePartitions : Command {};
}  // namespace partition

/**
 * @namespace message
 * @brief Commands related to reading and writing messages.
 */
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
class SendMessages : Command {};

}  // namespace message

/**
 * @namespace consumeroffset
 * @brief Commands related to managing server-side managed stream consumer offsets.
 */
namespace consumeroffset {
class GetConsumerOffset : Command {};
class StoreConsumerOffset : Command {};
}  // namespace consumeroffset

/**
 * @namespace consumergroup
 * @brief Commands related to managing server-side managed stream consumer groups.
 */
namespace consumergroup {
class GetConsumerGroup : Command {};
class GetConsumerGroups : Command {};
class CreateConsumerGroup : Command {};
class DeleteConsumerGroup : Command {};
class JoinConsumerGroup : Command {};
class LeaveConsumerGroup : Command {};
}  // namespace consumergroup

/**
 * @namespace system
 * @brief Commands related to global system status.
 */
namespace system {
class Ping : Command {};
class GetMe : Command {};
class GetClient : Command {};
class GetClients : Command {};
class GetStats : Command {};
}  // namespace system
}  // namespace command
}  // namespace iggy

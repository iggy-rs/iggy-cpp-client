#pragma once

#include <string>
#include "model.h"
#include "types.h"

namespace icp {

/**
 * @brief All supported commands in the Iggy protocol.
 *
 * Serialization-agnostic Command classes that you can send via an @ref icp::client::Client.
 */
namespace command {

class Command {
public:
    virtual ~Command() = default;
};

/**
 * @brief Commands related to managing Iggy streams.
 */
namespace stream {
class GetStream : Command {
private:
    icp::model::shared::Identifier streamId;

public:
    explicit GetStream(icp::model::shared::Identifier streamId)
        : streamId(streamId) {}

    icp::model::shared::Identifier getStreamId() const { return streamId; }
};

class GetStreams : Command {};
class CreateStream : Command {
private:
    icp::model::shared::Identifier streamId;
    std::string name;

public:
    CreateStream(icp::model::shared::Identifier streamId, std::string name)
        : streamId(streamId)
        , name(name) {}

    icp::model::shared::Identifier getStreamId() const { return streamId; }
    std::string getName() const { return name; }
};

class DeleteStream : Command {
private:
    icp::model::shared::Identifier streamId;

public:
    explicit DeleteStream(icp::model::shared::Identifier streamId)
        : streamId(streamId) {}

    icp::model::shared::Identifier getStreamId() const { return streamId; }
};
}  // namespace stream

/**
 * @brief Commands related to managing Iggy topics.
 */
namespace topic {
class GetTopic : Command {
private:
    icp::model::shared::Identifier streamId;
    icp::model::shared::Identifier topicId;

public:
    GetTopic(icp::model::shared::Identifier streamId, icp::model::shared::Identifier topicId)
        : streamId(streamId)
        , topicId(topicId) {}

    icp::model::shared::Identifier getStreamId() const { return streamId; }
    icp::model::shared::Identifier getTopicId() const { return topicId; }
};

class GetTopics : Command {
private:
    icp::model::shared::Identifier streamId;

public:
    explicit GetTopics(icp::model::shared::Identifier streamId)
        : streamId(streamId) {}

    icp::model::shared::Identifier getStreamId() const { return streamId; }
};

class CreateTopic : Command {
private:
    icp::model::shared::Identifier streamId;
    uint32_t topicId;
    uint32_t partitionsCount;
    std::optional<uint32_t> messageExpiry;
    std::string name;

public:
    CreateTopic(icp::model::shared::Identifier streamId,
                uint32_t topicId,
                uint32_t partitionsCount,
                std::optional<uint32_t> messageExpiry,
                std::string name)
        : streamId(streamId)
        , topicId(topicId)
        , partitionsCount(partitionsCount)
        , messageExpiry(messageExpiry)
        , name(name) {}

    icp::model::shared::Identifier getStreamId() const { return streamId; }
    uint32_t getTopicId() const { return topicId; }
    uint32_t getPartitionsCount() const { return partitionsCount; }
    std::optional<uint32_t> getMessageExpiry() const { return messageExpiry; }
    std::string getName() const { return name; }
};
}  // namespace topic

/**
 * @brief Commands related to managing Iggy partitions.
 */
namespace partition {
class CreatePartitions : Command {
private:
    icp::model::shared::Identifier streamId;
    icp::model::shared::Identifier topicId;
    uint32_t partitionsCount;

public:
    CreatePartitions(icp::model::shared::Identifier streamId, icp::model::shared::Identifier topicId, uint32_t partitionsCount)
        : streamId(streamId)
        , topicId(topicId)
        , partitionsCount(partitionsCount) {}

    icp::model::shared::Identifier getStreamId() const { return streamId; }
    icp::model::shared::Identifier getTopicId() const { return topicId; }
    uint32_t getPartitionsCount() const { return partitionsCount; }
};

class DeletePartitions : Command {
private:
    icp::model::shared::Identifier streamId;
    icp::model::shared::Identifier topicId;
    uint32_t partitionsCount;

public:
    DeletePartitions(icp::model::shared::Identifier streamId, icp::model::shared::Identifier topicId, uint32_t partitionsCount)
        : streamId(streamId)
        , topicId(topicId)
        , partitionsCount(partitionsCount) {}

    icp::model::shared::Identifier getStreamId() const { return streamId; }
    icp::model::shared::Identifier getTopicId() const { return topicId; }
    uint32_t getPartitionsCount() const { return partitionsCount; }
};
}  // namespace partition

/**
 * @brief Commands related to reading and writing messages.
 */
namespace message {
enum PollingKind { OFFSET = 1, TIMESTAMP = 2, FIRST = 3, LAST = 4, NEXT = 5 };
class PollingStrategy {
private:
    PollingKind kind;
    uint64_t value;

public:
    PollingStrategy(PollingKind kind, uint64_t value)
        : kind(kind)
        , value(value) {}
    PollingKind getKind() const { return kind; }
    uint64_t getValue() const { return value; }
};

class PollMessages : Command {
private:
    icp::model::shared::Consumer consumer;
    icp::model::shared::Identifier streamId;
    icp::model::shared::Identifier topicId;
    uint32_t partitionId;
    PollingStrategy strategy;
    uint32_t count;
    bool autoCommit;

public:
    PollMessages(icp::model::shared::Consumer consumer,
                 icp::model::shared::Identifier streamId,
                 icp::model::shared::Identifier topicId,
                 uint32_t partitionId,
                 PollingStrategy strategy,
                 uint32_t count,
                 bool autoCommit)
        : consumer(consumer)
        , streamId(streamId)
        , topicId(topicId)
        , partitionId(partitionId)
        , strategy(strategy)
        , count(count)
        , autoCommit(autoCommit) {}

    icp::model::shared::Consumer getConsumer() const { return consumer; }
    icp::model::shared::Identifier getStreamId() const { return streamId; }
    icp::model::shared::Identifier getTopicId() const { return topicId; }
    uint32_t getPartitionId() const { return partitionId; }
    PollingStrategy getStrategy() const { return strategy; }
    uint32_t getCount() const { return count; }
    bool getAutoCommit() const { return autoCommit; }
};

enum PartitioningKind { BALANCED = 1, PARITION_ID = 2, MESSAGES_KEY = 3 };

/**
 * @brief Layout for how to partition the messages being sent in @ref SendMessages.
 */
class Partitioning {
private:
    PartitioningKind kind;
    uint8_t length;
    std::vector<unsigned char> value;

public:
    Partitioning(PartitioningKind kind, uint8_t length, std::vector<unsigned char> value)
        : kind(kind)
        , length(length)
        , value(value) {}

    PartitioningKind getKind() const { return kind; }
    uint8_t getLength() const { return length; }
    std::vector<unsigned char> getValue() const { return value; }
};
class SendMessages : Command {};

}  // namespace message

/**
 * @brief Commands related to managing server-side managed stream consumer offsets.
 */
namespace consumeroffset {

/**
 * @brief Command to retrieve a server-side managed consumer offset.
 */
class GetConsumerOffset : Command {
private:
    icp::model::shared::Consumer consumer;
    icp::model::shared::Identifier streamId;
    icp::model::shared::Identifier topicId;
    uint32_t partitionId;

public:
    GetConsumerOffset(icp::model::shared::Consumer consumer,
                      icp::model::shared::Identifier streamId,
                      icp::model::shared::Identifier topicId,
                      uint32_t partitionId)
        : consumer(consumer)
        , streamId(streamId)
        , topicId(topicId)
        , partitionId(partitionId) {}

    icp::model::shared::Consumer getConsumer() const { return consumer; }
    icp::model::shared::Identifier getStreamId() const { return streamId; }
    icp::model::shared::Identifier getTopicId() const { return topicId; }
    uint32_t getPartitionId() const { return partitionId; }
};

/**
 * @brief Command to store a server-side managed consumer offset.
 */
class StoreConsumerOffset : Command {
private:
    icp::model::shared::Consumer consumer;
    icp::model::shared::Identifier streamId;
    icp::model::shared::Identifier topicId;
    uint32_t partitionId;
    uint64_t offset;

public:
    StoreConsumerOffset(icp::model::shared::Consumer consumer,
                        icp::model::shared::Identifier streamId,
                        icp::model::shared::Identifier topicId,
                        uint32_t partitionId,
                        uint64_t offset)
        : consumer(consumer)
        , streamId(streamId)
        , topicId(topicId)
        , partitionId(partitionId)
        , offset(offset) {}

    icp::model::shared::Consumer getConsumer() const { return consumer; }
    icp::model::shared::Identifier getStreamId() const { return streamId; }
    icp::model::shared::Identifier getTopicId() const { return topicId; }
    uint32_t getPartitionId() const { return partitionId; }
    uint64_t getOffset() const { return offset; }
};

}  // namespace consumeroffset

/**
 * @brief Commands related to managing server-side managed stream consumer groups.
 */
namespace consumergroup {

/**
 * @brief Command to retrieve details on a server-side managed consumer group.
 */
class GetConsumerGroup : Command {
private:
    icp::model::shared::Identifier streamId;
    icp::model::shared::Identifier topicId;
    uint32_t consumerGroupId;

public:
    GetConsumerGroup(icp::model::shared::Identifier streamId, icp::model::shared::Identifier topicId, uint32_t consumerGroupId)
        : streamId(streamId)
        , topicId(topicId)
        , consumerGroupId(consumerGroupId) {}

    icp::model::shared::Identifier getStreamId() const { return streamId; }
    icp::model::shared::Identifier getTopicId() const { return topicId; }
    uint32_t getConsumerGroupId() const { return consumerGroupId; }
};

/**
 * @brief Command to retrieve details on all server-side managed consumer groups for a given stream and topic.
 */
class GetConsumerGroups : Command {
private:
    icp::model::shared::Identifier streamId;
    icp::model::shared::Identifier topicId;

public:
    GetConsumerGroups(icp::model::shared::Identifier streamId, icp::model::shared::Identifier topicId)
        : streamId(streamId)
        , topicId(topicId) {}

    icp::model::shared::Identifier getStreamId() const { return streamId; }
    icp::model::shared::Identifier getTopicId() const { return topicId; }
};

class CreateConsumerGroup : Command {
private:
    icp::model::shared::Identifier streamId;
    icp::model::shared::Identifier topicId;
    uint32_t consumerGroupId;

public:
    CreateConsumerGroup(icp::model::shared::Identifier streamId, icp::model::shared::Identifier topicId, uint32_t consumerGroupId)
        : streamId(streamId)
        , topicId(topicId)
        , consumerGroupId(consumerGroupId) {}

    icp::model::shared::Identifier getStreamId() const { return streamId; }
    icp::model::shared::Identifier getTopicId() const { return topicId; }
    uint32_t getConsumerGroupId() const { return consumerGroupId; }
};

class DeleteConsumerGroup : Command {
private:
    icp::model::shared::Identifier streamId;
    icp::model::shared::Identifier topicId;
    uint32_t consumerGroupId;

public:
    DeleteConsumerGroup(icp::model::shared::Identifier streamId, icp::model::shared::Identifier topicId, uint32_t consumerGroupId)
        : streamId(streamId)
        , topicId(topicId)
        , consumerGroupId(consumerGroupId) {}

    icp::model::shared::Identifier getStreamId() const { return streamId; }
    icp::model::shared::Identifier getTopicId() const { return topicId; }
    uint32_t getConsumerGroupId() const { return consumerGroupId; }
};

class JoinConsumerGroup : Command {
private:
    icp::model::shared::Identifier streamId;
    icp::model::shared::Identifier topicId;
    uint32_t consumerGroupId;

public:
    JoinConsumerGroup(icp::model::shared::Identifier streamId, icp::model::shared::Identifier topicId, uint32_t consumerGroupId)
        : streamId(streamId)
        , topicId(topicId)
        , consumerGroupId(consumerGroupId) {}

    icp::model::shared::Identifier getStreamId() const { return streamId; }
    icp::model::shared::Identifier getTopicId() const { return topicId; }
    uint32_t getConsumerGroupId() const { return consumerGroupId; }
};

class LeaveConsumerGroup : Command {
private:
    icp::model::shared::Identifier streamId;
    icp::model::shared::Identifier topicId;
    uint32_t consumerGroupId;

public:
    LeaveConsumerGroup(icp::model::shared::Identifier streamId, icp::model::shared::Identifier topicId, uint32_t consumerGroupId)
        : streamId(streamId)
        , topicId(topicId)
        , consumerGroupId(consumerGroupId) {}

    icp::model::shared::Identifier getStreamId() const { return streamId; }
    icp::model::shared::Identifier getTopicId() const { return topicId; }
    uint32_t getConsumerGroupId() const { return consumerGroupId; }
};

}  // namespace consumergroup

/**
 * @brief Commands related to global system state.
 */
namespace system {
/**
 * @brief Simple ping command to check if the server is alive.
 */
class Ping : Command {};

/**
 * @brief Command to get information about the calling client.
 */
class GetMe : Command {};

/**
 * @brief Command to get information about a specific client.
 */
class GetClient : Command {
private:
    uint32_t clientId;

public:
    explicit GetClient(uint32_t clientId)
        : clientId(clientId) {}

    uint32_t getClientId() const { return clientId; }
};

/**
 * @brief Command to get the full list of clients on the server.
 */
class GetClients : Command {};

/**
 * @brief Command to get information about the server's performance
 */
class GetStats : Command {};
}  // namespace system
}  // namespace command
}  // namespace icp

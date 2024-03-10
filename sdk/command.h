#pragma once

#include <string>
#include "model.h"
#include "types.h"

namespace iggy {

/**
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
 * @brief Commands related to managing Iggy streams.
 */
namespace stream {
class GetStream : Command {
private:
    iggy::model::shared::Identifier streamId;

public:
    explicit GetStream(iggy::model::shared::Identifier streamId)
        : streamId(streamId) {}

    iggy::model::shared::Identifier getStreamId() const { return streamId; }
};

class GetStreams : Command {};
class CreateStream : Command {
private:
    iggy::model::shared::Identifier streamId;
    std::string name;

public:
    CreateStream(iggy::model::shared::Identifier streamId, std::string name)
        : streamId(streamId)
        , name(name) {}

    iggy::model::shared::Identifier getStreamId() const { return streamId; }
    std::string getName() const { return name; }
};

class DeleteStream : Command {
private:
    iggy::model::shared::Identifier streamId;

public:
    explicit DeleteStream(iggy::model::shared::Identifier streamId)
        : streamId(streamId) {}

    iggy::model::shared::Identifier getStreamId() const { return streamId; }
};
}  // namespace stream

/**
 * @brief Commands related to managing Iggy topics.
 */
namespace topic {
class GetTopic : Command {
private:
    iggy::model::shared::Identifier streamId;
    iggy::model::shared::Identifier topicId;

public:
    GetTopic(iggy::model::shared::Identifier streamId, iggy::model::shared::Identifier topicId)
        : streamId(streamId)
        , topicId(topicId) {}

    iggy::model::shared::Identifier getStreamId() const { return streamId; }
    iggy::model::shared::Identifier getTopicId() const { return topicId; }
};

class GetTopics : Command {
private:
    iggy::model::shared::Identifier streamId;

public:
    explicit GetTopics(iggy::model::shared::Identifier streamId)
        : streamId(streamId) {}

    iggy::model::shared::Identifier getStreamId() const { return streamId; }
};

class CreateTopic : Command {
private:
    iggy::model::shared::Identifier streamId;
    uint32_t topicId;
    uint32_t partitionsCount;
    std::optional<uint32_t> messageExpiry;
    std::string name;

public:
    CreateTopic(iggy::model::shared::Identifier streamId,
                uint32_t topicId,
                uint32_t partitionsCount,
                std::optional<uint32_t> messageExpiry,
                std::string name)
        : streamId(streamId)
        , topicId(topicId)
        , partitionsCount(partitionsCount)
        , messageExpiry(messageExpiry)
        , name(name) {}

    iggy::model::shared::Identifier getStreamId() const { return streamId; }
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
    iggy::model::shared::Identifier streamId;
    iggy::model::shared::Identifier topicId;
    uint32_t partitionsCount;

public:
    CreatePartitions(iggy::model::shared::Identifier streamId, iggy::model::shared::Identifier topicId, uint32_t partitionsCount)
        : streamId(streamId)
        , topicId(topicId)
        , partitionsCount(partitionsCount) {}

    iggy::model::shared::Identifier getStreamId() const { return streamId; }
    iggy::model::shared::Identifier getTopicId() const { return topicId; }
    uint32_t getPartitionsCount() const { return partitionsCount; }
};

class DeletePartitions : Command {
private:
    iggy::model::shared::Identifier streamId;
    iggy::model::shared::Identifier topicId;
    uint32_t partitionsCount;

public:
    DeletePartitions(iggy::model::shared::Identifier streamId, iggy::model::shared::Identifier topicId, uint32_t partitionsCount)
        : streamId(streamId)
        , topicId(topicId)
        , partitionsCount(partitionsCount) {}

    iggy::model::shared::Identifier getStreamId() const { return streamId; }
    iggy::model::shared::Identifier getTopicId() const { return topicId; }
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
    iggy::model::shared::Consumer consumer;
    iggy::model::shared::Identifier streamId;
    iggy::model::shared::Identifier topicId;
    uint32_t partitionId;
    PollingStrategy strategy;
    uint32_t count;
    bool autoCommit;

public:
    PollMessages(iggy::model::shared::Consumer consumer,
                 iggy::model::shared::Identifier streamId,
                 iggy::model::shared::Identifier topicId,
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

    iggy::model::shared::Consumer getConsumer() const { return consumer; }
    iggy::model::shared::Identifier getStreamId() const { return streamId; }
    iggy::model::shared::Identifier getTopicId() const { return topicId; }
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
    iggy::model::shared::Consumer consumer;
    iggy::model::shared::Identifier streamId;
    iggy::model::shared::Identifier topicId;
    uint32_t partitionId;

public:
    GetConsumerOffset(iggy::model::shared::Consumer consumer,
                      iggy::model::shared::Identifier streamId,
                      iggy::model::shared::Identifier topicId,
                      uint32_t partitionId)
        : consumer(consumer)
        , streamId(streamId)
        , topicId(topicId)
        , partitionId(partitionId) {}

    iggy::model::shared::Consumer getConsumer() const { return consumer; }
    iggy::model::shared::Identifier getStreamId() const { return streamId; }
    iggy::model::shared::Identifier getTopicId() const { return topicId; }
    uint32_t getPartitionId() const { return partitionId; }
};

/**
 * @brief Command to store a server-side managed consumer offset.
 */
class StoreConsumerOffset : Command {
private:
    iggy::model::shared::Consumer consumer;
    iggy::model::shared::Identifier streamId;
    iggy::model::shared::Identifier topicId;
    uint32_t partitionId;
    uint64_t offset;

public:
    StoreConsumerOffset(iggy::model::shared::Consumer consumer,
                        iggy::model::shared::Identifier streamId,
                        iggy::model::shared::Identifier topicId,
                        uint32_t partitionId,
                        uint64_t offset)
        : consumer(consumer)
        , streamId(streamId)
        , topicId(topicId)
        , partitionId(partitionId)
        , offset(offset) {}

    iggy::model::shared::Consumer getConsumer() const { return consumer; }
    iggy::model::shared::Identifier getStreamId() const { return streamId; }
    iggy::model::shared::Identifier getTopicId() const { return topicId; }
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
    iggy::model::shared::Identifier streamId;
    iggy::model::shared::Identifier topicId;
    uint32_t consumerGroupId;

public:
    GetConsumerGroup(iggy::model::shared::Identifier streamId, iggy::model::shared::Identifier topicId, uint32_t consumerGroupId)
        : streamId(streamId)
        , topicId(topicId)
        , consumerGroupId(consumerGroupId) {}

    iggy::model::shared::Identifier getStreamId() const { return streamId; }
    iggy::model::shared::Identifier getTopicId() const { return topicId; }
    uint32_t getConsumerGroupId() const { return consumerGroupId; }
};

/**
 * @brief Command to retrieve details on all server-side managed consumer groups for a given stream and topic.
 */
class GetConsumerGroups : Command {
private:
    iggy::model::shared::Identifier streamId;
    iggy::model::shared::Identifier topicId;

public:
    GetConsumerGroups(iggy::model::shared::Identifier streamId, iggy::model::shared::Identifier topicId)
        : streamId(streamId)
        , topicId(topicId) {}

    iggy::model::shared::Identifier getStreamId() const { return streamId; }
    iggy::model::shared::Identifier getTopicId() const { return topicId; }
};

class CreateConsumerGroup : Command {
private:
    iggy::model::shared::Identifier streamId;
    iggy::model::shared::Identifier topicId;
    uint32_t consumerGroupId;

public:
    CreateConsumerGroup(iggy::model::shared::Identifier streamId, iggy::model::shared::Identifier topicId, uint32_t consumerGroupId)
        : streamId(streamId)
        , topicId(topicId)
        , consumerGroupId(consumerGroupId) {}

    iggy::model::shared::Identifier getStreamId() const { return streamId; }
    iggy::model::shared::Identifier getTopicId() const { return topicId; }
    uint32_t getConsumerGroupId() const { return consumerGroupId; }
};

class DeleteConsumerGroup : Command {
private:
    iggy::model::shared::Identifier streamId;
    iggy::model::shared::Identifier topicId;
    uint32_t consumerGroupId;

public:
    DeleteConsumerGroup(iggy::model::shared::Identifier streamId, iggy::model::shared::Identifier topicId, uint32_t consumerGroupId)
        : streamId(streamId)
        , topicId(topicId)
        , consumerGroupId(consumerGroupId) {}

    iggy::model::shared::Identifier getStreamId() const { return streamId; }
    iggy::model::shared::Identifier getTopicId() const { return topicId; }
    uint32_t getConsumerGroupId() const { return consumerGroupId; }
};

class JoinConsumerGroup : Command {
private:
    iggy::model::shared::Identifier streamId;
    iggy::model::shared::Identifier topicId;
    uint32_t consumerGroupId;

public:
    JoinConsumerGroup(iggy::model::shared::Identifier streamId, iggy::model::shared::Identifier topicId, uint32_t consumerGroupId)
        : streamId(streamId)
        , topicId(topicId)
        , consumerGroupId(consumerGroupId) {}

    iggy::model::shared::Identifier getStreamId() const { return streamId; }
    iggy::model::shared::Identifier getTopicId() const { return topicId; }
    uint32_t getConsumerGroupId() const { return consumerGroupId; }
};

class LeaveConsumerGroup : Command {
private:
    iggy::model::shared::Identifier streamId;
    iggy::model::shared::Identifier topicId;
    uint32_t consumerGroupId;

public:
    LeaveConsumerGroup(iggy::model::shared::Identifier streamId, iggy::model::shared::Identifier topicId, uint32_t consumerGroupId)
        : streamId(streamId)
        , topicId(topicId)
        , consumerGroupId(consumerGroupId) {}

    iggy::model::shared::Identifier getStreamId() const { return streamId; }
    iggy::model::shared::Identifier getTopicId() const { return topicId; }
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
}  // namespace iggy

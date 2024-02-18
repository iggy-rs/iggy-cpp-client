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
    PollMessages(iggy::model::shared::Consumer consumer, iggy::model::shared::Identifier streamId,
                 iggy::model::shared::Identifier topicId, uint32_t partitionId, PollingStrategy strategy,
                 uint32_t count, bool autoCommit)
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
class Partitioning {};
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
 * @brief Commands related to global system state.
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

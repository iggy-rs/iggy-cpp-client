#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "types.h"

namespace iggy {
namespace model {

class Model {
public:
    virtual ~Model() = default;
};

namespace shared {
/**
 * @brief Discriminator to allow correct decoding of identifier bytes.
 */
enum IdKind { NUMERIC = 1, STRING = 2 };

/**
 * @brief An identifier for a stream or a topic.
 */
class Identifier : Model {
private:
    IdKind kind;
    uint8_t length;
    std::vector<unsigned char> value;

public:
    Identifier(IdKind kind, uint8_t length, std::vector<unsigned char> value)
        : kind(kind)
        , length(length)
        , value(value) {}
    IdKind getKind() { return kind; }
    uint8_t getLength() { return length; }
    std::vector<unsigned char> getValue() { return value; }
};

enum ConsumerKind { CONSUMER = 1, CONSUMER_GROUP = 2 };

/**
 * @brief A consumer of a message stream; may be part of a consumer group.
 */
class Consumer : Model {
private:
    ConsumerKind kind;
    uint32_t id;

public:
    Consumer(ConsumerKind kind, uint32_t id)
        : kind(kind)
        , id(id) {}
    ConsumerKind getKind() { return kind; }
    uint32_t getId() { return id; }
};

class ConsumerOffsetInfo : Model {

};
};

namespace message {

/**
 * @brief Current state of the message on the server.
 */
enum MessageState {
    AVAILABLE = 1,
    UNAVAILABLE = 10,
    POISONED = 20,
    MARKED_FOR_DELETION = 30
};

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

class HeaderValue : Model {
private:
    HeaderKind kind;
    std::vector<unsigned char> value;
public:
    HeaderValue(HeaderKind kind, std::vector<unsigned char> value)
        : kind(kind)
        , value(value) {}

    HeaderKind getKind() const { return kind; }
    std::vector<unsigned char> getValue() const { return value; }
};

class Message : Model {
private:
    uint64_t offset;
    MessageState state;
    uint64_t timestamp;
    uint128_t id;
    uint32_t checksum;
    std::unordered_map<HeaderKey, HeaderValue> headers;
    uint32_t length;
    std::vector<unsigned char> payload;
public:
    uint64_t getOffset() const { return offset; }
    MessageState getState() const { return state; }
    uint64_t getTimestamp() const { return timestamp; }
    uint128_t getId() const { return id; }
    uint32_t getChecksum() const { return checksum; }
    std::unordered_map<HeaderKey, HeaderValue> getHeaders() const { return headers; }
    uint32_t getLength() const { return length; }
    std::vector<unsigned char> getPayload() const { return payload; }
};

class PolledMessages : Model {
private:
    uint32_t partition_id;
    uint64_t current_offset;
    std::vector<Message> messages;
public:
    uint32_t getPartitionId() const { return partition_id; }
    uint64_t getCurrentOffset() const { return current_offset; }
    std::vector<Message> getMessages() const { return messages; }
};

}  // namespace message

/**
 * @brief Models related to global system state.
 */
namespace system {
/**
 * @class Stats
 * @brief Model class holding server performance statistics.
 *
 * This class is used to represent the result of the GetStats call,
 * which gets a snapshot of latest server performance statistics.
 *
 * @see [GetStats definition](https://docs.iggy.rs/specification/binary#get-stats)
 * @see [stats.rs](https://github.com/iggy-rs/iggy/blob/master/iggy/src/models/stats.rs)
 */
class Stats : Model {
private:
    pid_t process_id;
    percent_t cpu_usage;
    byte_cnt_t memory_usage;
    byte_cnt_t total_memory;
    byte_cnt_t available_memory;
    time_val_t run_time;
    time_val_t start_time;
    byte_cnt_t read_bytes;
    byte_cnt_t written_bytes;
    byte_cnt_t messages_size_bytes;
    obj_cnt_t streams_count;
    obj_cnt_t topics_count;
    obj_cnt_t partitions_count;
    obj_cnt_t segments_count;
    msg_cnt_t messages_count;
    obj_cnt_t clients_count;
    obj_cnt_t consumer_groups_count;
    std::string hostname;
    std::string os_name;
    std::string os_version;
    std::string kernel_version;

public:
    // TODO: when we add the full constructor ensure convertToUTF8() called on all strings
    Stats() = default;

    /// @brief Get the server process ID (PID)
    pid_t getProcessId() const { return process_id; }

    /// @brief Get the server process CPU usage.
    percent_t getCpuUsage() const { return cpu_usage; }

    /// @brief Get the server total memory usage.
    byte_cnt_t getMemoryUsage() const { return memory_usage; }

    /// @brief Get the server total memory.
    byte_cnt_t getTotalMemory() const { return total_memory; }

    /// @brief Get the server available memory.
    byte_cnt_t getAvailableMemory() const { return available_memory; }

    /// @brief Get how long the server has been running, in seconds.
    time_val_t getRunTime() const { return run_time; }

    /// @brief Get the server start time, in seconds since the UNIX epoch.
    time_val_t getStartTime() const { return start_time; }

    /// @brief Get the number of bytes read since process start.
    byte_cnt_t getReadBytes() const { return read_bytes; }

    /// @brief Get the number of bytes written since process start.
    byte_cnt_t getWrittenBytes() const { return written_bytes; }

    /// @brief Get the total size of all messages processed.
    byte_cnt_t getMessagesSizeBytes() const { return messages_size_bytes; }

    /// @brief Get the total number of streams in the server.
    obj_cnt_t getStreamsCount() const { return streams_count; }

    /// @brief Get the total number of topics on the server.
    obj_cnt_t getTopicsCount() const { return topics_count; }

    /// @brief Get the total number of topic partitions on the server across all topics.
    obj_cnt_t getPartitionsCount() const { return partitions_count; }

    /// @brief Get the total number of fixed-size segments used for topic storage on disk.
    obj_cnt_t getSegmentsCount() const { return segments_count; }

    /// @brief Get the total number of messages processed by the server across all topics.
    msg_cnt_t getMessagesCount() const { return messages_count; }

    /// @brief Get the total number of active connected clients.
    obj_cnt_t getClientsCount() const { return clients_count; }

    /// @brief Get the total number of active consumer groups on the server.
    obj_cnt_t getConsumerGroupsCount() const { return consumer_groups_count; }

    /// @brief Get the name of the host that the server process is running on.
    std::string getHostname() const { return hostname; }

    /// @brief Get the name of the operating system that the server process is running on.
    std::string getOsName() const { return os_name; }

    /// @brief Get the version of the operating system that the server process is running on.
    std::string getOsVersion() const { return os_version; }

    /// @brief Get the version of the OS kernel that the server process is running on.
    std::string getKernelVersion() const { return kernel_version; }
};

}  // namespace system
}  // namespace model
}  // namespace iggy

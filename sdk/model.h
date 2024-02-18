#pragma once

#include <sys/types.h>
#include <cstdint>
#include <iostream>
#include <string>

namespace iggy {
namespace models {

/// @brief Mapping for fractional values between 0 and 1.
typedef float percent_t;

/// @brief Mapping for absolute times and time durations; currently in seconds.
typedef uint64_t time_val_t;

/// @brief Mapping for values that measure byte counts.
typedef uint64_t byte_cnt_t;

/// @brief Mapping for values that measure object counts.
typedef uint32_t obj_cnt_t;

/// @brief Mapping for values that measure (potentially very large) message counts.
typedef uint64_t msg_cnt_t;

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
class Stats {
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

    /// @brief Helper that writes the Stats object into a stream.
    friend std::ostream& operator<<(std::ostream& os, const Stats& stats);

    /// @brief Helper that reads the Stats object from a stream.
    friend std::istream& operator>>(std::istream& is, Stats& stats);
};

/// @brief Helper that writes the Stats object into a stream.
std::ostream& operator<<(std::ostream& os, const Stats& stats);

/// @brief Helper that reads the Stats object from a stream.
std::istream& operator>>(std::istream& is, Stats& stats);

}  // namespace models
}  // namespace iggy

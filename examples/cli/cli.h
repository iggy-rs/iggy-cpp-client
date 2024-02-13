#pragma once

#include <functional>
#include <initializer_list>
#include <string>

namespace iggy {
namespace cli {

// standard CLI flags
const std::initializer_list<char const* const> VERBOSE_FLAGS = {"-v", "--verbose"};
const std::initializer_list<char const* const> HELP_FLAGS = {"-?", "-h", "--help"};

class CommandContext {
public:
    CommandContext(const std::string& username, const std::function<std::string*()>& callback);
    ~CommandContext() = default;
};

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(const CommandContext& ctx) const = 0;
};

class PingCommand : public Command {
public:
    void execute(const CommandContext& ctx) const override;
};

class StatusCommand : public Command {
public:
    void execute(const CommandContext& ctx) const override;
};

void print_usage(const std::string& program_name);
}  // namespace cli
}  // namespace iggy

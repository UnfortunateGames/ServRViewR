#include <algorithm>
#include <parsers.hh>

char lower(char x) { return std::tolower(x); }

std::optional<unsigned int>
convert_to_unsigned_int(std::string target) {
    std::stringstream ss(target);
    unsigned int out;
    ss >> out;
    if (ss.fail()) return std::nullopt;
    return out;
}

std::expected<std::vector<Command>, CommandReturn>
parse_commands(std::string commands) {
    std::vector<Command> command_list{};
    std::vector<std::array<size_t, 4>> separate_indexes{{0, 0, 0, 0}};
    std::transform(commands.begin(), commands.end(), commands.begin(), lower);
    if (
        auto x = commands.find_first_not_of(' ');
        x != std::string::npos
    ) separate_indexes[0][0] = x;
    while (true) {
        if (
            auto x = commands.find(' ', separate_indexes.back()[0]);
            x != std::string::npos
            && commands.at(x - 1) != ';'
        ) separate_indexes.back()[1] = x;
        if (
            auto x = commands.find(';', separate_indexes.back()[0]);
            x != std::string::npos
        ) {
            if (separate_indexes.back()[1] == 0) separate_indexes.back()[1] = x;
            else if (
                auto y = commands.find_first_not_of(' ', separate_indexes.back()[1]);
                y != std::string::npos
                && commands.at(y) != ';'
            ) {
                separate_indexes.back()[2] = y;
                separate_indexes.back()[3] = x;
            }
            separate_indexes.push_back({0});
            if (
                auto y = commands.find_first_not_of(' ', x + 1);
                y != std::string::npos
            ) separate_indexes.back()[0] = y;
        } else {
            if (separate_indexes.back()[1] == 0) separate_indexes.back()[1] = commands.length();
            else if (
                auto x = commands.find_first_not_of(' ', separate_indexes.back()[1]);
                x != std::string::npos
            ) {
                separate_indexes.back()[2] = x;
                separate_indexes.back()[3] = commands.length();
            }
            break;
        }
    }
    std::string_view command;
    for (int i{0uz}; i < separate_indexes.size(); i++) {
        command = commands.substr(
            separate_indexes[i][0],
            separate_indexes[i][1]
        );
        if (command.empty()) continue;
        if (command.compare("help") == 0) {
            command_list.push_back(Command{
                Command::CMD::Help
            });
        } else if (command.compare("exit") == 0) {
            command_list.push_back(Command{
                Command::CMD::Exit
            });
        } else if (command.compare("call") == 0) {
            if (separate_indexes[i][2] == 0)
                return std::unexpected{CommandReturn{
                    CommandReturn::ReturnSignal::InvalidParameter,
                    "No parameter was given."
                }};
            std::string parameter = commands.substr(
                separate_indexes[i][2],
                separate_indexes[i][3]
            );
            if (
                auto result = convert_to_unsigned_int(parameter);
                result.has_value()
            ) command_list.push_back(Command{
                Command::CMD::Call, result.value()
            });
            else return std::unexpected{CommandReturn{
                CommandReturn::ReturnSignal::InvalidParameter,
                std::format("{} is not a valid ID.", parameter)
            }};
        } else if (command.compare("run") == 0) {
            if (separate_indexes[i][2] == 0)
                return std::unexpected{CommandReturn{
                    CommandReturn::ReturnSignal::InvalidParameter,
                    "No parameter was given."
                }};
            std::string parameter = commands.substr(
                separate_indexes[i][2],
                separate_indexes[i][3]
            );
            if (
                auto result = convert_to_unsigned_int(parameter);
                result.has_value()
            ) command_list.push_back(Command{
                Command::CMD::Run, result.value()
            });
            else return std::unexpected{CommandReturn{
                CommandReturn::ReturnSignal::InvalidParameter,
                std::format("{} is not a valid ID.", parameter)
            }};
        } else if (command.compare("stop") == 0) {
            if (separate_indexes[i][2] == 0)
                return std::unexpected{CommandReturn{
                    CommandReturn::ReturnSignal::InvalidParameter,
                    "No parameter was given."
                }};
            std::string parameter = commands.substr(
                separate_indexes[i][2],
                separate_indexes[i][3]
            );
            if (
                auto result = convert_to_unsigned_int(parameter);
                result.has_value()
            ) command_list.push_back(Command{
                Command::CMD::Stop, result.value()
            });
            else return std::unexpected{CommandReturn{
                CommandReturn::ReturnSignal::InvalidParameter,
                std::format("{} is not a valid ID.", parameter)
            }};
        } else if (command.compare("shutdown") == 0) {
            if (separate_indexes[i][2] == 0)
                return std::unexpected{CommandReturn{
                    CommandReturn::ReturnSignal::InvalidParameter,
                    "No parameter was given."
                }};
            std::string parameter = commands.substr(
                separate_indexes[i][2],
                separate_indexes[i][3]
            );
            if (
                auto result = convert_to_unsigned_int(parameter);
                result.has_value()
            ) command_list.push_back(Command{
                Command::CMD::Shutdown, result.value()
            });
            else return std::unexpected{CommandReturn{
                CommandReturn::ReturnSignal::InvalidParameter,
                std::format("{} is not a valid ID.", parameter)
            }};
        } else if (command.compare("activate") == 0) {
            if (separate_indexes[i][2] == 0)
                return std::unexpected{CommandReturn{
                    CommandReturn::ReturnSignal::InvalidParameter,
                    "No parameter was given."
                }};
            std::string parameter = commands.substr(
                separate_indexes[i][2],
                separate_indexes[i][3]
            );
            if (
                auto result = convert_to_unsigned_int(parameter);
                result.has_value()
            ) command_list.push_back(Command{
                Command::CMD::Activate, result.value()
            });
            else return std::unexpected{CommandReturn{
                CommandReturn::ReturnSignal::InvalidParameter,
                std::format("{} is not a valid ID.", parameter)
            }};
        } else if (command.compare("add") == 0) {
            command_list.push_back(Command{
                Command::CMD::Add
            });
        } else if (command.compare("remove") == 0) {
            if (separate_indexes[i][2] == 0)
                return std::unexpected{CommandReturn{
                    CommandReturn::ReturnSignal::InvalidParameter,
                    "No parameter was given."
                }};
            std::string parameter = commands.substr(
                separate_indexes[i][2],
                separate_indexes[i][3]
            );
            if (
                auto result = convert_to_unsigned_int(parameter);
                result.has_value()
            ) command_list.push_back(Command{
                Command::CMD::Remove, result.value()
            });
            else return std::unexpected{CommandReturn{
                CommandReturn::ReturnSignal::InvalidParameter,
                std::format("{} is not a valid ID.", parameter)
            }};
        } else if (command.compare("edit") == 0) {
            if (separate_indexes[i][2] == 0)
                return std::unexpected{CommandReturn{
                    CommandReturn::ReturnSignal::InvalidParameter,
                    "No parameter was given."
                }};
            std::string parameter = commands.substr(
                separate_indexes[i][2],
                separate_indexes[i][3]
            );
            if (
                auto result = convert_to_unsigned_int(parameter);
                result.has_value()
            ) command_list.push_back(Command{
                Command::CMD::Edit, result.value()
            });
            else return std::unexpected{CommandReturn{
                CommandReturn::ReturnSignal::InvalidParameter,
                std::format("{} is not a valid ID.", parameter)
            }};
        } else if (command.compare("read") == 0) {
            command_list.push_back(Command{
                Command::CMD::Read
            });
        } else if (command.compare("wait") == 0) {
            if (separate_indexes[i][2] == 0)
                return std::unexpected{CommandReturn{
                    CommandReturn::ReturnSignal::InvalidParameter,
                    "No parameter was given."
                }};
            std::string parameter = commands.substr(
                separate_indexes[i][2],
                separate_indexes[i][3]
            );
            if (
                auto result = convert_to_unsigned_int(parameter);
                result.has_value()
            ) command_list.push_back(Command{
                Command::CMD::Wait, result.value()
            });
            else return std::unexpected{CommandReturn{
                CommandReturn::ReturnSignal::InvalidParameter,
                std::format("{} is not a valid ID.", parameter)
            }};
        } else return std::unexpected{CommandReturn{
            CommandReturn::ReturnSignal::InvalidCommand,
            std::format("{} is not a valid command!", command)
        }};
    }
    return command_list;
}
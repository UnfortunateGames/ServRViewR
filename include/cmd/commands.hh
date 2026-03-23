#pragma once
#include <optional>
#include <thread>
#include <chrono>
#include <expected>
#include <fstream>
#include <iostream>
#include "data.hh"
#include "../inpututil.hh"

Server make_server(ID unique_id);

std::optional<CommandReturn>
with_server(
    uint32_t parameter,
    ServerList& server_list,
    std::vector<std::string>& output,
    void (*pLambda)(Server& server, std::vector<std::string>& output)
);

std::expected<Server, std::string>
read_to_server(std::string path);

struct Command {
    enum CMD {
        Help, Exit,
        Call, Run, Stop, Shutdown, Activate,
        Add, Remove, Edit,
        Read,
        Wait
    };
    const CMD command;
    const uint32_t parameter = 0; // 0 is a default, and an invalid ID
    std::optional<CommandReturn>
    execute(ServerList& server_list, std::vector<std::string>& output) {
        bool has_found_server;
        size_t index{0uz};
        ID given_uid{1};
        auto call_lambda = [](
            Server& server,
            std::vector<std::string>& output
        ) {
            server.call(output);
        };
        auto run_lambda = [](
            Server& server,
            std::vector<std::string>& output
        ) {
            server.status = ServerStatus::RUNNING;
        };
        auto stop_lambda = [](
            Server& server,
            std::vector<std::string>& output
        ) {
            if (server.status != ServerStatus::RUNNING) {
                output.push_back(
                    std::format(
                        "{} is currently not running.",
                        server.name
                    )
                );
            } else server.status = ServerStatus::UP;
        };
        auto shutdown_lambda = [](
            Server& server,
            std::vector<std::string>& output
        ) {
            if (server.status == ServerStatus::DOWN) {
                output.push_back(
                    std::format(
                        "Server is already down.",
                        server.name
                    )
                );
                return;
            }
            server.status = ServerStatus::DOWN;
        };
        auto activate_lambda = [](
            Server& server,
            std::vector<std::string>& output
        ) {
            if (server.status != ServerStatus::DOWN) {
                output.push_back(
                    std::format(
                        "{} is already up/running.",
                        server.name
                    )
                );
                return;
            }
            server.status = ServerStatus::UP;
        };
        switch (this->command) {
            case CMD::Help:
                output.push_back(HELP_MESSAGE);
                break;
            case CMD::Exit:
                return CommandReturn{
                    CommandReturn::ReturnSignal::ExitSignal
                };
            case CMD::Call:
                return with_server(parameter, server_list, output, call_lambda);
            case CMD::Run:
                return with_server(parameter, server_list, output, run_lambda);
            case CMD::Stop:
                return with_server(parameter, server_list, output, stop_lambda);
            case CMD::Shutdown:
                return with_server(parameter, server_list, output, shutdown_lambda);
            case CMD::Activate:
                return with_server(parameter, server_list, output, activate_lambda);
            case CMD::Add:
                for (Server& server : server_list.self) {
                    if (
                        given_uid
                            .self
                        <
                        server
                            .unique_id
                            .self
                    )
                        given_uid = ID{
                            server.unique_id.self + 1
                        };
                }
                server_list.self.push_back(
                    make_server(given_uid)
                );
                break;
            case CMD::Remove:
                for (; index < server_list.self.size(); index++) {
                    if (
                        server_list
                            .self[index]
                            .unique_id
                            .self
                    ==
                    parameter
                    ) {
                        has_found_server = true;
                        break;
                    }
                }
                if (has_found_server) {
                    for (; server_list.self.size() > index;) {
                        server_list.self[index++] = server_list.self[index];
                    }
                    server_list.self.pop_back();
                } else
                    return CommandReturn{
                        CommandReturn::ReturnSignal::InvalidParameter,
                        std::format(
                            "Could not find server with ID: {}",
                            (int) parameter
                        )
                    };
                break;
            case CMD::Edit:
                for (Server& server : server_list.self) {
                    if (
                        server
                            .unique_id
                            .self
                        ==
                        parameter
                    ) {
                        server = make_server(
                            server.unique_id
                        );
                    }
                }
                break;
            case CMD::Read:
                std::cout
                    << "Please input the file path."
                    << std::endl;
                if (
                    auto result = read_to_server(
                        inputf("-> ")
                    );
                    result.has_value()
                ) {
                    server_list.self.push_back(result.value());
                } else {
                    output.push_back(result.error());
                }
                break;
            case CMD::Wait:
                std::this_thread::sleep_for(
                    std::chrono::seconds{
                        parameter
                    }
                );
                break;
        }
        return std::nullopt;
    }
};

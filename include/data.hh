#pragma once
#include <cstdint>
#include <string>
#include <vector>

static const std::string HELP_MESSAGE = R"(
- ServRViewR -

Fun Fact: This message isn't a replica to
          ServRLookR, I actually forgot to
          implement it.

' Available Commands '

HELP            - Prints this message.
EXIT            - Exits the program.
CALL     [ID]   - Calls server with ID.
RUN      [ID]   - Runs the server to be called every command run
STOP     [ID]   - Stops a running server.
SHUTDOWN [ID]   - Makes a server go down.
ACTIVATE [ID]   - Makes a server go up.
ADD             - You will be prompted to create one server
REMOVE   [ID]   - Removes given ID.
EDIT     [ID]   - You will be prompted to overwrite the server
READ            - You will be prompted for the file path.
WAIT    [TIME]  - Waits given TIME in seconds.
)"; // FINISH THIS PLEASE BRO

struct ID {
    uint32_t self;
};

enum ServerStatus {
    UP, RUNNING, DOWN
};

/*
 * file example:
 * 
 * NAME
 * MESSAGE
 * ERR_MESSAGE
 * UID
 * 
 */

struct Server {
    std::string name;
    ServerStatus status;
    std::string message;
    std::string err_message;
    ID unique_id;
    void call(std::vector<std::string>& output) {
        if (status == ServerStatus::DOWN)
            output.push_back(err_message);
        else output.push_back(message);
    }
    Server& operator=(const Server& other) {
        name = other.name;
        status = other.status;
        message = other.message;
        err_message = other.message;
        unique_id = other.unique_id;
        return *this;
    }
};

struct ServerList {
    std::vector<Server> self;
    Server* find(ID uid) {
        // Replace this with an STL
        for (Server& server : self) {
            if (server.unique_id.self == uid.self)
                return &server;
        }
        return nullptr;
    }
    void print_servers() {
        for (Server& server : self) {
            std::cout
                << "- " << server.name << " -" << std::endl
                << "Status - ";
            switch (server.status) {
                case ServerStatus::UP:
                    std::cout << "UP" << std::endl;
                case ServerStatus::RUNNING:
                    std::cout << "RUNNING" << std::endl;
                case ServerStatus::DOWN:
                    std::cout << "DOWN" << std::endl;
            }
            std::cout
                << "Unique ID - " << (int) server.unique_id.self << std::endl;
        }
    }
};

struct CommandReturn {
    enum ReturnSignal {
        InvalidCommand,
        InvalidParameter,
        ExitSignal
    };
    const ReturnSignal signal;
    std::string message = ""; // Yeah maybe this makes sense. &str aint supposed to be there.
};

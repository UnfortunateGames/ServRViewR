#include <iostream>
#include <data.hh>
#include <parsers.hh>

int main() {
    ServerList server_list{};
    std::vector<std::string> output;
    std::string input;
    std::vector<Command> command_list;
    std::cout << "- ServRViewR -\n" << std::endl;
    while (true) {
        server_list.print_servers();
        for (std::string line : output)
            std::cout << line << std::endl;
        output.clear();
        input = inputf("\n-> ");
        if (
            auto x = parse_commands(input);
            x.has_value()
        ) {
            command_list.clear();
            for (Command command : x.value()) {
                command_list.push_back(command);
            }
        } else {
            if (x.error().signal == CommandReturn::ReturnSignal::InvalidCommand)
                output.push_back("[INVALID COMMAND!]");
            else if (x.error().signal == CommandReturn::ReturnSignal::InvalidParameter)
                output.push_back("[INVALID PARAMETER!]");
            output.push_back(x.error().message);
            continue;
        }
        for (Command& command : command_list) {
            auto x = command.execute(server_list, output);
            if (x.has_value()) {
                if (x.value().signal == CommandReturn::ReturnSignal::InvalidParameter)
                    output.push_back("[INVALID PARAMETER!]");
                else if (x.value().signal == CommandReturn::ReturnSignal::ExitSignal)
                    return 0;
                output.push_back(x.value().message);
            }
        }
    }
    return 0;
}

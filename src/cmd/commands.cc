#include <sstream>
#include <cmd/commands.hh>
#include <inpututil.hh>

Server make_server(ID unique_id) {
    return Server{
        inputf("Enter the server's name -> "),
        ServerStatus::UP,
        inputf("Enter it's message -> "),
        inputf("Enter it's error message -> "),
        unique_id
    };
}

std::optional<CommandReturn>
with_server(
    uint32_t parameter,
    ServerList& server_list,
    std::vector<std::string>& output,
    void (*pLambda)(Server& server, std::vector<std::string>& output)
) {
    if (parameter == 0) return CommandReturn{
        CommandReturn::ReturnSignal::InvalidParameter,
        "0, or no parameter was used. Please enter a valid ID."
    };
    if (
        Server* server = server_list.find(ID{parameter});
        server != nullptr
    ) {
        pLambda(*server, output);
        return std::nullopt;
    } else CommandReturn{
        CommandReturn::ReturnSignal::InvalidParameter,
        std::format(
            "Could not find server with ID {}",
            (int) parameter
        )
    };
    return std::nullopt;
}

std::optional<std::string>
read_line(
    std::ifstream& file,
    std::string err_message,
    std::string& out
) {
    if (!std::getline(file, out))
        return err_message;
    return std::nullopt;
}

std::expected<Server, std::string>
read_to_server(std::string path) {
    std::ifstream file(path);
    if (!file.is_open())
        return std::unexpected{
            std::format(
                "'{}' is not a valid filepath",
                path
            )
        };
    Server out;
    if (
        auto result = read_line(
            file,
            std::format(
                "{} is either empty or corrupted",
                path
            ),
            out.name
        );
        result.has_value()
    ) return std::unexpected{result.value()};
    if (
        auto result = read_line(
            file,
            std::format(
                "{} is incomplete. [Could not fetch message]",
                path
            ),
            out.message
        );
        result.has_value()
    ) return std::unexpected{result.value()};
    if (
        auto result = read_line(
            file,
            std::format(
                "{} is incomplete. [Could not fetch error message]",
                path
            ),
            out.err_message
        );
        result.has_value()
    ) return std::unexpected{result.value()};
    std::string buffer;
    if (
        auto result = read_line(
            file,
            std::format(
                "{} is incomplete. [Could not fetch unique ID]",
                path
            ),
            buffer
        );
        result.has_value()
    ) return std::unexpected{result.value()};
    uint32_t uid;
    std::stringstream ss(buffer);
    ss >> uid;
    if (ss.fail())
        return std::unexpected{"Failed to parse unique ID."};
    out.unique_id = ID{uid};
    return out;
}

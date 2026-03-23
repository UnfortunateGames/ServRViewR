#pragma once
#include <expected>
#include <string>
#include <vector>
#include "cmd/commands.hh"
#include "data.hh"

char lower(char x);

std::optional<unsigned int>
convert_to_unsigned_int(std::string target);

std::expected<std::vector<Command>, CommandReturn>
parse_commands(std::string commands);

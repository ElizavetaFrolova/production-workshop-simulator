#pragma once

#include <string>
#include <vector>

#include "models/ParsedInput.hpp"

struct ParseResult {
    bool success = false;
    std::string errorLine;
    ParsedInput input;
};

class Parser {
public:
    ParseResult parseFile(const std::string& filename) const;

private:
    std::vector<std::string> splitBySpaces(const std::string& line) const;

    bool parseInteger(const std::string& text, int maxValue, int& value) const;
};
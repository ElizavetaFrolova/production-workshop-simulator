#include "parser/Parser.hpp"

#include <fstream>
#include <sstream>

std::vector<std::string> Parser::splitBySpaces(const std::string& line) const {
    std::vector<std::string> parts;

    std::stringstream stream(line);
    std::string part;

    while (stream >> part) {
        parts.push_back(part);
    }

    return parts;
}

bool Parser::parseInteger(
    const std::string& text,
    int maxValue,
    int& value
) const {
    if (text.empty() || maxValue < 0) {
        return false;
    }

    int result = 0;

    for (char symbol : text) {
        if (symbol < '0' || symbol > '9') {
            return false;
        }

        result = result * 10 + (symbol - '0');

        if (result > maxValue) {
            return false;
        }
    }

    value = result;
    return true;
}

ParseResult Parser::parseFile(const std::string& filename) const {
    ParseResult result;

    std::ifstream input(filename);

    if (!input.is_open()) {
        result.success = false;
        result.errorLine = "";
        return result;
    }

    std::string line;

    if (!std::getline(input, line)) {
        result.success = false;
        result.errorLine = "";
        return result;
    }

    std::vector<std::string> parts = splitBySpaces(line);

    if (parts.size() != 2) {
        result.success = false;
        result.errorLine = line;
        return result;
    }

    int M = 0;
    int N = 0;

    if (!parseInteger(parts[0], 100, M) ||
        !parseInteger(parts[1], 100, N) ||M < 1 ||N < 1) {
        result.success = false;
        result.errorLine = line;
        return result;
    }

    result.input.M = M;
    result.input.N = N;

    result.input.times.assign(result.input.M - 1
        , std::vector<int>(result.input.N, 0));

    for (int operation = 0; operation < result.input.M - 1; ++operation) {
        if (!std::getline(input, line)) {
            result.success = false;
            result.errorLine = "";
            return result;
        }

        parts = splitBySpaces(line);

        if (static_cast<int>(parts.size()) != result.input.N) {
            result.success = false;
            result.errorLine = line;
            return result;
        }

        for (int machine = 0; machine < result.input.N; ++machine) {
            int time = 0;

            if (!parseInteger(parts[machine], 10000, time)) {
                result.success = false;
                result.errorLine = line;
                return result;
            }

            result.input.times[operation][machine] = time;
        }
    }

    result.input.initialQueues.assign(
        result.input.N,
        std::vector<int>()
    );

    int totalItems = 0;

    for (int machine = 0; machine < result.input.N; ++machine) {
        if (!std::getline(input, line)) {
            result.success = false;
            result.errorLine = "";
            return result;
        }

        parts = splitBySpaces(line);

        if (parts.empty()) {
            result.success = false;
            result.errorLine = line;
            return result;
        }

        int queueSize = 0;

        if (!parseInteger(parts[0], 100000, queueSize)) {
            result.success = false;
            result.errorLine = line;
            return result;
        }

        if (static_cast<int>(parts.size()) != queueSize + 1) {
            result.success = false;
            result.errorLine = line;
            return result;
        }

        totalItems += queueSize;

        if (totalItems > 100000) {
            result.success = false;
            result.errorLine = line;
            return result;
        }

        result.input.initialQueues[machine].reserve(queueSize);

        for (int position = 0; position < queueSize; ++position) {
            int itemType = 0;

            if (!parseInteger( parts[position + 1]
                , result.input.M - 2, itemType)) {
                result.success = false;
                result.errorLine = line;
                return result;
            }

            result.input.initialQueues[machine].push_back(itemType);
        }
    }

    while (std::getline(input, line)) {
        if (!line.empty()) {
            result.success = false;
            result.errorLine = line;
            return result;
        }
    }

    result.success = true;
    return result;
}
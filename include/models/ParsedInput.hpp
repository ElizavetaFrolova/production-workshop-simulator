#pragma once

#include <vector>

struct ParsedInput {
    int M = 0;
    int N = 0;

    std::vector<std::vector<int>> times;
    std::vector<std::vector<int>> initialQueues;
};
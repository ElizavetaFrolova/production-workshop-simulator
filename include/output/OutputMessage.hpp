#pragma once

#include <string>

struct OutputMessage {
    long long time = 0;
    int priority = 0;
    int order = 0;
    std::string text;
};
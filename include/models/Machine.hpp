#pragma once

#include <deque>

struct Machine {
    int id = 0;

    std::deque<int> queue;

    bool busy = false;
    int currentItem = -1;

    long long queueWork = 0;
};
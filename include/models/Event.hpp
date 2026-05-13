#pragma once

struct Event {
    long long time = 0;
    int machineId = 0;
    int itemId = 0;
    int operation = 0;
};

struct EventCompare {
    bool operator()(const Event& left, const Event& right) const {
        if (left.time != right.time) {
            return left.time > right.time;
        }

        if (left.machineId != right.machineId) {
            return left.machineId > right.machineId;
        }

        return left.itemId > right.itemId;
    }
};
#pragma once

#include <string>
#include <vector>

#include "output/OutputMessage.hpp"

class OutputWriter {
private:
    std::vector<OutputMessage> messages;
    int nextOrder = 0;

    void addMessage(long long time, int priority, const std::string& text);

public:
    void addStart(long long t, int k, int i, int j);
    void addFinish(long long t, int k, int i, int j);
    void addWait(long long t, int k, int i, int j, int p);
    void addReady(long long t, int k, int j);
    void addStop(long long t);

    void printAll() const;
    void printAll(std::ostream& output) const;
};
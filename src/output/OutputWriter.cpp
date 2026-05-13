#include "output/OutputWriter.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>

namespace {

bool compareMessages(const OutputMessage& left, const OutputMessage& right) {
    if (left.time != right.time) {
        return left.time < right.time;
    }

    if (left.priority != right.priority) {
        return left.priority < right.priority;
    }

    return left.order < right.order;
}

}

void OutputWriter::addStart(long long t, int k, int i, int j) {
    std::ostringstream out;
    out << "start " << t << ' ' << k << ' ' << i << ' ' << j;

    addMessage(t, 1, out.str());
}

void OutputWriter::addFinish(long long t, int k, int i, int j) {
    std::ostringstream out;
    out << "finish " << t << ' ' << k << ' ' << i << ' ' << j;

    addMessage(t, 0, out.str());
}

void OutputWriter::addWait(long long t, int k, int i, int j, int p) {
    std::ostringstream out;
    out << "wait " << t << ' ' << k << ' ' << i << ' ' << j << ' ' << p;

    addMessage(t, 2, out.str());
}

void OutputWriter::addReady(long long t, int k, int j) {
    std::ostringstream out;
    out << "ready " << t << ' ' << k << ' ' << j;

    addMessage(t, 3, out.str());
}

void OutputWriter::addStop(long long t) {
    std::ostringstream out;
    out << "stop " << t;

    addMessage(t, 4, out.str());
}

void OutputWriter::printAll(std::ostream& output) const {
    std::vector<OutputMessage> sortedMessages = messages;

    std::sort(sortedMessages.begin()
    , sortedMessages.end(), compareMessages);

    for (const auto& message : sortedMessages) {
        output << message.text << '\n';
    }
}

void OutputWriter::printAll() const {
    printAll(std::cout);
}

void OutputWriter::addMessage(long long time, int priority, const std::string& text) {
    messages.push_back(OutputMessage{time
        , priority, nextOrder, text});

    ++nextOrder;
}
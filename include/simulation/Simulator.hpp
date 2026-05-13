#pragma once

#include <queue>
#include <vector>

#include "models/Event.hpp"
#include "models/Item.hpp"
#include "models/Machine.hpp"
#include "models/ParsedInput.hpp"
#include "output/OutputWriter.hpp"

class Simulator {
private:
    int M = 0;
    int N = 0;

    std::vector<std::vector<int>> times;
    std::vector<std::vector<int>> initialQueues;

    std::vector<Item> items;
    std::vector<Machine> machines;

    std::priority_queue<Event, std::vector<Event>, EventCompare> events;

    OutputWriter output;

    int totalItems = 0;
    int readyCount = 0;

    long long lastTime = 0;

public:
    explicit Simulator(const ParsedInput& input);

    void run();
    void run(std::ostream& outputStream);

private:
    void initializeItemsAndMachines();

    void startInitialProcessing();
    void processEventsAtTime(long long time);

    void startProcessing(int machineId, int itemId, long long time);
    void finishProcessing(const Event& event);

    void moveFinishedItem(const Event& event, long long time);

    int chooseMachineForNextOperation() const;

    void addToQueue(int machineId, int itemId, long long time);
    void tryStartNextItem(int machineId, long long time);

};
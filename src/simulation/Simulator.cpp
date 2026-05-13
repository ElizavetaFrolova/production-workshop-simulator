#include "simulation/Simulator.hpp"

#include <algorithm>

#include <iostream>

namespace {

bool compareEventsByMachineAndItem(const Event& left, const Event& right) {
    if (left.machineId != right.machineId) {
        return left.machineId < right.machineId;
    }

    return left.itemId < right.itemId;
}

}

Simulator::Simulator(const ParsedInput& input): M(input.M)
, N(input.N), times(input.times)
, initialQueues(input.initialQueues)
,machines(static_cast<size_t>(input.N)) {
    for (int i = 0; i < N; ++i) {
        machines[i].id = i;
    }

    initializeItemsAndMachines();
}

void Simulator::run() {
    run(std::cout);
}

void Simulator::run(std::ostream& outputStream) {
    if (totalItems == 0) {
        output.addStop(0);
        output.printAll(outputStream);
        return;
    }

    startInitialProcessing();

    while (!events.empty() && readyCount < totalItems) {
        long long currentTime = events.top().time;
        lastTime = currentTime;

        processEventsAtTime(currentTime);
    }

    output.addStop(lastTime);
    output.printAll(outputStream);
}

void Simulator::initializeItemsAndMachines() {
    int nextItemId = 0;

    for (int machineId = 0; machineId < N; ++machineId) {
        for (int type : initialQueues[machineId]) {
            Item item;
            item.id = nextItemId;
            item.type = type;

            items.push_back(item);

            machines[machineId].queue.push_back(nextItemId);
            machines[machineId].queueWork += times[type][machineId];

            ++nextItemId;
        }
    }

    totalItems = nextItemId;
}

void Simulator::startInitialProcessing() {
    for (int machineId = 0; machineId < N; ++machineId) {
        tryStartNextItem(machineId, 0);
    }
}

void Simulator::processEventsAtTime(long long time) {
    std::vector<Event> currentEvents;

    while (!events.empty() && events.top().time == time) {
        currentEvents.push_back(events.top());
        events.pop();
    }

    std::sort(currentEvents.begin()
    , currentEvents.end(), compareEventsByMachineAndItem);

    for (const Event& event : currentEvents) {
        finishProcessing(event);
    }

    for (const Event& event : currentEvents) {
        moveFinishedItem(event, time);
    }

    for (const Event& event : currentEvents) {
        tryStartNextItem(event.machineId, time);
    }
}

void Simulator::startProcessing(int machineId, int itemId, long long time) {
    Machine& machine = machines[machineId];
    Item& item = items[itemId];

    machine.busy = true;
    machine.currentItem = itemId;

    output.addStart(time, item.id, item.type, machineId);

    long long finishTime = time + times[item.type][machineId];

    events.push(Event{finishTime
        , machineId, itemId, item.type});
}

void Simulator::finishProcessing(const Event& event) {
    Machine& machine = machines[event.machineId];

    machine.busy = false;
    machine.currentItem = -1;

    output.addFinish(event.time, event.itemId
        , event.operation, event.machineId);
}

void Simulator::moveFinishedItem(const Event& event, long long time) {
    Item& item = items[event.itemId];

    item.type = event.operation + 1;

    if (item.type == M - 1) {
        ++readyCount;

        output.addReady(time, item.id, event.machineId);

        return;
    }

    int selectedMachine = chooseMachineForNextOperation();

    if (!machines[selectedMachine].busy && machines[selectedMachine].queue.empty()) {
        startProcessing(selectedMachine, item.id, time);
    } else {
        addToQueue(selectedMachine, item.id, time);
    }
}

int Simulator::chooseMachineForNextOperation() const {
    int bestMachine = 0;

    for (int machineId = 1; machineId < N; ++machineId) {
        if (machines[machineId].queueWork < machines[bestMachine].queueWork) {
            bestMachine = machineId;
        }
    }

    return bestMachine;
}

void Simulator::addToQueue(int machineId, int itemId, long long time) {
    Machine& machine = machines[machineId];
    Item& item = items[itemId];

    int queueSizeBefore = static_cast<int>(machine.queue.size());

    output.addWait(time, item.id, item.type, machineId, queueSizeBefore);

    machine.queue.push_back(item.id);
    machine.queueWork += times[item.type][machineId];
}

void Simulator::tryStartNextItem(int machineId, long long time) {
    Machine& machine = machines[machineId];

    if (machine.busy || machine.queue.empty()) {
        return;
    }

    int itemId = machine.queue.front();
    machine.queue.pop_front();

    Item& item = items[itemId];

    machine.queueWork -= times[item.type][machineId];

    startProcessing(machineId, itemId, time);
}
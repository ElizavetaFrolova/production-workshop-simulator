#include "simulation/Simulator.hpp"

#include <gtest/gtest.h>

#include <sstream>
#include <string>
#include <vector>

namespace {

std::string runSimulation(const ParsedInput& input) {
    Simulator simulator(input);

    std::ostringstream output;
    simulator.run(output);

    return output.str();
}

}

TEST(SimulatorTest, StopsAtZeroWhenThereAreNoItems) {
    ParsedInput input;
    input.M = 2;
    input.N = 2;
    input.times = {
        {3, 5}
    };
    input.initialQueues = {
        {},
        {}
    };

    EXPECT_EQ(
        runSimulation(input),
        "stop 0\n"
    );
}

TEST(SimulatorTest, ProcessesSingleItemOnSingleMachine) {
    ParsedInput input;
    input.M = 2;
    input.N = 1;
    input.times = {
        {5}
    };
    input.initialQueues = {
        {0}
    };

    EXPECT_EQ(
        runSimulation(input),
        "start 0 0 0 0\n"
        "finish 5 0 0 0\n"
        "ready 5 0 0\n"
        "stop 5\n"
    );
}

TEST(SimulatorTest, ChoosesMachineWithSmallerNumberWhenQueueWorksAreEqual) {
    ParsedInput input;
    input.M = 3;
    input.N = 2;
    input.times = {
        {1, 1},
        {5, 6}
    };
    input.initialQueues = {
        {0},
        {}
    };

    EXPECT_EQ(
        runSimulation(input),
        "start 0 0 0 0\n"
        "finish 1 0 0 0\n"
        "start 1 0 1 0\n"
        "finish 6 0 1 0\n"
        "ready 6 0 0\n"
        "stop 6\n"
    );
}

TEST(SimulatorTest, ProcessesZeroDurationOperations) {
    ParsedInput input;
    input.M = 3;
    input.N = 1;
    input.times = {
        {0},
        {0}
    };
    input.initialQueues = {
        {0}
    };

    EXPECT_EQ(
        runSimulation(input),
        "finish 0 0 0 0\n"
        "finish 0 0 1 0\n"
        "start 0 0 0 0\n"
        "start 0 0 1 0\n"
        "ready 0 0 0\n"
        "stop 0\n"
    );
}

TEST(SimulatorTest, RunsFullExampleWithSpecificationOrder) {
    ParsedInput input;
    input.M = 3;
    input.N = 2;
    input.times = {
        {3, 5},
        {4, 6}
    };
    input.initialQueues = {
        {0, 1, 0},
        {1, 0}
    };

    EXPECT_EQ(
    runSimulation(input),
    "start 0 0 0 0\n"
    "start 0 3 1 1\n"
    "finish 3 0 0 0\n"
    "start 3 1 1 0\n"
    "wait 3 0 1 1 1\n"
    "finish 6 3 1 1\n"
    "start 6 4 0 1\n"
    "ready 6 3 1\n"
    "finish 7 1 1 0\n"
    "start 7 2 0 0\n"
    "ready 7 1 0\n"
    "finish 10 2 0 0\n"
    "start 10 2 1 0\n"
    "finish 11 4 0 1\n"
    "start 11 0 1 1\n"
    "wait 11 4 1 0 0\n"
    "finish 14 2 1 0\n"
    "start 14 4 1 0\n"
    "ready 14 2 0\n"
    "finish 17 0 1 1\n"
    "ready 17 0 1\n"
    "finish 18 4 1 0\n"
    "ready 18 4 0\n"
    "stop 18\n"
    );
}
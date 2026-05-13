#include "output/OutputWriter.hpp"

#include <gtest/gtest.h>

#include <sstream>
#include <string>

namespace {

std::string getOutput(const OutputWriter& writer) {
    std::ostringstream output;
    writer.printAll(output);
    return output.str();
}

}

TEST(OutputWriterTest, PrintsStartMessage) {
    OutputWriter writer;

    writer.addStart(0, 5, 1, 2);

    EXPECT_EQ(
        getOutput(writer),
        "start 0 5 1 2\n"
    );
}

TEST(OutputWriterTest, PrintsFinishMessage) {
    OutputWriter writer;

    writer.addFinish(10, 3, 0, 1);

    EXPECT_EQ(
        getOutput(writer),
        "finish 10 3 0 1\n"
    );
}

TEST(OutputWriterTest, PrintsWaitMessage) {
    OutputWriter writer;

    writer.addWait(7, 4, 1, 0, 3);

    EXPECT_EQ(
        getOutput(writer),
        "wait 7 4 1 0 3\n"
    );
}

TEST(OutputWriterTest, PrintsReadyMessage) {
    OutputWriter writer;

    writer.addReady(15, 8, 2);

    EXPECT_EQ(
        getOutput(writer),
        "ready 15 8 2\n"
    );
}

TEST(OutputWriterTest, PrintsStopMessage) {
    OutputWriter writer;

    writer.addStop(20);

    EXPECT_EQ(
        getOutput(writer),
        "stop 20\n"
    );
}

TEST(OutputWriterTest, SortsMessagesByTime) {
    OutputWriter writer;

    writer.addStop(10);
    writer.addStart(0, 0, 0, 0);
    writer.addFinish(5, 0, 0, 0);

    EXPECT_EQ(
        getOutput(writer),
        "start 0 0 0 0\n"
        "finish 5 0 0 0\n"
        "stop 10\n"
    );
}

TEST(OutputWriterTest, SortsMessagesWithSameTimeByPriority) {
    OutputWriter writer;

    writer.addReady(4, 3, 1);
    writer.addWait(4, 0, 1, 1, 1);
    writer.addStart(4, 4, 0, 1);
    writer.addFinish(4, 3, 1, 1);

    EXPECT_EQ(
        getOutput(writer),
        "finish 4 3 1 1\n"
        "start 4 4 0 1\n"
        "wait 4 0 1 1 1\n"
        "ready 4 3 1\n"
    );
}

TEST(OutputWriterTest, KeepsInsertionOrderForSameTimeAndSamePriority) {
    OutputWriter writer;

    writer.addStart(0, 0, 0, 0);
    writer.addStart(0, 3, 1, 1);
    writer.addStart(0, 5, 0, 2);

    EXPECT_EQ(
        getOutput(writer),
        "start 0 0 0 0\n"
        "start 0 3 1 1\n"
        "start 0 5 0 2\n"
    );
}

TEST(OutputWriterTest, StopGoesAfterOtherMessagesAtSameTime) {
    OutputWriter writer;

    writer.addStop(5);
    writer.addReady(5, 1, 0);
    writer.addFinish(5, 1, 1, 0);
    writer.addStart(5, 2, 0, 1);
    writer.addWait(5, 3, 1, 1, 0);

    EXPECT_EQ(
        getOutput(writer),
        "finish 5 1 1 0\n"
        "start 5 2 0 1\n"
        "wait 5 3 1 1 0\n"
        "ready 5 1 0\n"
        "stop 5\n"
    );
}
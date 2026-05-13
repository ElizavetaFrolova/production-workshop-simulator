#include "parser/Parser.hpp"

#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

namespace {

void writeFile(const std::string& filename, const std::string& content) {
    std::ofstream output(filename);
    ASSERT_TRUE(output.is_open());

    output << content;
}

ParseResult parseText(const std::string& content) {
    const std::string filename = "parser_test_input.txt";

    writeFile(filename, content);

    Parser parser;
    ParseResult result = parser.parseFile(filename);

    std::remove(filename.c_str());

    return result;
}

}  // namespace

TEST(ParserTest, ParsesValidInput) {
    ParseResult result = parseText(
        "3 2\n"
        "3 5\n"
        "4 6\n"
        "3 0 1 0\n"
        "2 1 0\n"
    );

    ASSERT_TRUE(result.success);

    EXPECT_EQ(result.input.M, 3);
    EXPECT_EQ(result.input.N, 2);

    ASSERT_EQ(result.input.times.size(), 2);
    ASSERT_EQ(result.input.times[0].size(), 2);
    ASSERT_EQ(result.input.times[1].size(), 2);

    EXPECT_EQ(result.input.times[0][0], 3);
    EXPECT_EQ(result.input.times[0][1], 5);
    EXPECT_EQ(result.input.times[1][0], 4);
    EXPECT_EQ(result.input.times[1][1], 6);

    ASSERT_EQ(result.input.initialQueues.size(), 2);

    EXPECT_EQ(result.input.initialQueues[0], std::vector<int>({0, 1, 0}));
    EXPECT_EQ(result.input.initialQueues[1], std::vector<int>({1, 0}));
}

TEST(ParserTest, AllowsExtraSpacesBetweenNumbers) {
    ParseResult result = parseText(
        "3   2\n"
        "3   5\n"
        "4   6\n"
        "3   0   1   0\n"
        "2   1   0\n"
    );

    ASSERT_TRUE(result.success);

    EXPECT_EQ(result.input.M, 3);
    EXPECT_EQ(result.input.N, 2);
    EXPECT_EQ(result.input.times[0][0], 3);
    EXPECT_EQ(result.input.times[0][1], 5);
    EXPECT_EQ(result.input.initialQueues[0], std::vector<int>({0, 1, 0}));
    EXPECT_EQ(result.input.initialQueues[1], std::vector<int>({1, 0}));
}

TEST(ParserTest, FailsWhenFirstLineHasWrongNumberCount) {
    ParseResult result = parseText(
        "3 2 1\n"
        "3 5\n"
        "4 6\n"
        "3 0 1 0\n"
        "2 1 0\n"
    );

    ASSERT_FALSE(result.success);
    EXPECT_EQ(result.errorLine, "3 2 1");
}

TEST(ParserTest, FailsWhenMIsOutOfRange) {
    ParseResult result = parseText(
        "0 2\n"
        "3 5\n"
        "4 6\n"
        "3 0 1 0\n"
        "2 1 0\n"
    );

    ASSERT_FALSE(result.success);
    EXPECT_EQ(result.errorLine, "0 2");
}

TEST(ParserTest, FailsWhenNIsOutOfRange) {
    ParseResult result = parseText(
        "3 101\n"
        "3 5\n"
        "4 6\n"
        "3 0 1 0\n"
        "2 1 0\n"
    );

    ASSERT_FALSE(result.success);
    EXPECT_EQ(result.errorLine, "3 101");
}

TEST(ParserTest, FailsWhenTimeLineHasWrongNumberCount) {
    ParseResult result = parseText(
        "3 2\n"
        "3\n"
        "4 6\n"
        "3 0 1 0\n"
        "2 1 0\n"
    );

    ASSERT_FALSE(result.success);
    EXPECT_EQ(result.errorLine, "3");
}

TEST(ParserTest, FailsWhenTimeIsGreaterThanLimit) {
    ParseResult result = parseText(
        "3 2\n"
        "3 10001\n"
        "4 6\n"
        "3 0 1 0\n"
        "2 1 0\n"
    );

    ASSERT_FALSE(result.success);
    EXPECT_EQ(result.errorLine, "3 10001");
}

TEST(ParserTest, FailsWhenNumberContainsLetter) {
    ParseResult result = parseText(
        "3 2\n"
        "3 x\n"
        "4 6\n"
        "3 0 1 0\n"
        "2 1 0\n"
    );

    ASSERT_FALSE(result.success);
    EXPECT_EQ(result.errorLine, "3 x");
}

TEST(ParserTest, FailsWhenNumberIsNegative) {
    ParseResult result = parseText(
        "3 2\n"
        "3 -5\n"
        "4 6\n"
        "3 0 1 0\n"
        "2 1 0\n"
    );

    ASSERT_FALSE(result.success);
    EXPECT_EQ(result.errorLine, "3 -5");
}

TEST(ParserTest, FailsWhenQueueLineIsEmpty) {
    ParseResult result = parseText(
        "3 2\n"
        "3 5\n"
        "4 6\n"
        "\n"
        "2 1 0\n"
    );

    ASSERT_FALSE(result.success);
    EXPECT_EQ(result.errorLine, "");
}

TEST(ParserTest, FailsWhenQueueHasLessItemsThanDeclared) {
    ParseResult result = parseText(
        "3 2\n"
        "3 5\n"
        "4 6\n"
        "3 0 1\n"
        "2 1 0\n"
    );

    ASSERT_FALSE(result.success);
    EXPECT_EQ(result.errorLine, "3 0 1");
}

TEST(ParserTest, FailsWhenQueueHasMoreItemsThanDeclared) {
    ParseResult result = parseText(
        "3 2\n"
        "3 5\n"
        "4 6\n"
        "2 0 1 0\n"
        "2 1 0\n"
    );

    ASSERT_FALSE(result.success);
    EXPECT_EQ(result.errorLine, "2 0 1 0");
}

TEST(ParserTest, FailsWhenItemTypeIsOutOfRange) {
    ParseResult result = parseText(
        "3 2\n"
        "3 5\n"
        "4 6\n"
        "3 0 2 0\n"
        "2 1 0\n"
    );

    ASSERT_FALSE(result.success);
    EXPECT_EQ(result.errorLine, "3 0 2 0");
}

TEST(ParserTest, ParsesMEqualOneWithEmptyQueues) {
    ParseResult result = parseText(
        "1 2\n"
        "0\n"
        "0\n"
    );

    ASSERT_TRUE(result.success);

    EXPECT_EQ(result.input.M, 1);
    EXPECT_EQ(result.input.N, 2);
    EXPECT_TRUE(result.input.times.empty());

    ASSERT_EQ(result.input.initialQueues.size(), 2);
    EXPECT_TRUE(result.input.initialQueues[0].empty());
    EXPECT_TRUE(result.input.initialQueues[1].empty());
}

TEST(ParserTest, FailsMEqualOneWithItemInQueue) {
    ParseResult result = parseText(
        "1 2\n"
        "1 0\n"
        "0\n"
    );

    ASSERT_FALSE(result.success);
    EXPECT_EQ(result.errorLine, "1 0");
}

TEST(ParserTest, FailsWhenThereIsExtraNonEmptyLine) {
    ParseResult result = parseText(
        "3 2\n"
        "3 5\n"
        "4 6\n"
        "3 0 1 0\n"
        "2 1 0\n"
        "extra\n"
    );

    ASSERT_FALSE(result.success);
    EXPECT_EQ(result.errorLine, "extra");
}

TEST(ParserTest, AllowsEmptyLinesAtEnd) {
    ParseResult result = parseText(
        "3 2\n"
        "3 5\n"
        "4 6\n"
        "3 0 1 0\n"
        "2 1 0\n"
        "\n"
        "\n"
    );

    ASSERT_TRUE(result.success);
}
add_executable(all_tests
    ParserTest.cpp
    OutputWriterTest.cpp
    SimulatorTest.cpp
)

target_link_libraries(all_tests PRIVATE
    simulator_core
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(all_tests)
#include <iostream>

#include "parser/Parser.hpp"
#include "simulation/Simulator.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        return 1;
    }

    Parser parser;
    ParseResult parseResult = parser.parseFile(argv[1]);

    if (!parseResult.success) {
        std::cout << parseResult.errorLine << '\n';
        return 0;
    }

    Simulator simulator(parseResult.input);
    simulator.run();

    return 0;
}
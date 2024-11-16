
#include "InterpreterDriver/InterpreterDriver.h"

#include <iostream>
#include <exception>
#include <chrono>
#include <fstream>
#include <iterator>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

const int EXIT_DATAERR = 65;
const int EXIT_SOFTWARE = 70;


namespace cosmos{
    auto InterpreterDriver::run_script(const char* const script_file) -> int{
        const auto source = ([&]() -> std::string {
            try{
                std::ifstream in(script_file, std::ios::in);
                in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
                return std::string{std::istreambuf_iterator<char>{in}, std::istreambuf_iterator<char>{}};
            } catch (std::exception& e) {
                return "";
            }
        })();
        
        if (source.empty()) return EXIT_DATAERR;

        this->interpret(source);

        if(had_error) return EXIT_DATAERR;
        if(had_runtime_error) return EXIT_SOFTWARE;
        return 0;
    }


    



    InterpreterDriver::InterpreterDriver() {}

} //namespace cosmos


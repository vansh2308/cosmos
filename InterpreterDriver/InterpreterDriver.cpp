
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


    void InterpreterDriver::run_REPL(){
        std::string line;
        std::cout << "# Greetings! Welcome to COSMOS\n" << std::endl;

        while(std::cout << ">" && std::getline(std::cin, line)){
            this->interpret(line);
            had_error = false;
            had_runtime_error = false;
        }

        std::cout << "Goodbye!\n";
    }

    namespace {
        class InterpreterError : std::exception {};
        
        auto scan(const std::string& source) -> std::vector<int>{
            // WIP: Implement scanning
        }

        auto parse() -> int {
            // WIP: Implement Parsing 
        }
    }



    void InterpreterDriver::interpret(const std::string& source){
        try{
            #ifdef PERF_DEBUG
                auto scan_start_time = std::chrono::high_resolution_clock::now();
                auto tokens = scan(source);
                auto parse_start_time = std::chrono::high_resolution_clock::now();
                // lines.emplace_back(parse(tokens));
                auto eval_start_time = std::chrono::high_resolution_clock::now();
                // evaluator.evaluateStmts(lines.back());
                auto eval_end_time = std::chrono::high_resolution_clock::now();

                std::cout << "Scanning took: " << static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>( parse_start_time - scan_start_time).count()) << "us\n";

                std::cout << "Parsing took: " << static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(eval_start_time - parse_start_time).count()) << "us\n";

                std::cout << "Evaluation took: " << static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(eval_end_time - eval_start_time).count()) << "us\n";
            #else
                // lines.emplace_back(parse(scan(source)));
                // evaluator.evaluateStmts(lines.back());
            #endif // PERF_DEBUG
                // if (eReporter.getStatus() != LoxStatus::OK) {
                    // eReporter.printToStdErr();
                // }
        } catch (const InterpreterError& e) {
            had_error = true;
            return;
        } 
    }

    InterpreterDriver::InterpreterDriver() {}

} //namespace cosmos


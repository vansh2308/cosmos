
#ifndef CLOX_INTERPRETERDRIVER_INTERPRETERDRIVER_H
#define CLOX_INTERPRETERDRIVER_INTERPRETERDRIVER_H
#pragma once

#include <string>
#include <vector>




namespace cosmos{
    struct InterpreterDriver{
        public:
        InterpreterDriver();
        int run_script(const char* script);
        void run_REPL();

        private:
        void interpret(const std::string& source);

        // WIP: Add evaluator, debugger, AST 

        bool had_error = false;
        bool had_runtime_error = false;
    };
}



#endif

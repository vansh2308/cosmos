
#ifndef COSMOS_INTERPRETERDRIVER_INTERPRETERDRIVER_H
#define COSMOS_INTERPRETERDRIVER_INTERPRETERDRIVER_H
#pragma once

#include <string>
#include <vector>

#include "../AST/nodeTypes.h"
#include "../Evaluator/evaluator.h"


namespace cosmos{
    struct InterpreterDriver{
        public:
        InterpreterDriver();
        auto run_script(const char* script) -> int; 
        void run_REPL();

        private:
        void interpret(const std::string& source);

        // WIP: Add evaluator, debugger, AST 
        Evaluator::Evaluator evaluator;
        std::vector<std::vector<AST::StmtPtrVariant>> lines;

        bool had_error = false;
        bool had_runtime_error = false;
    };
}


#endif

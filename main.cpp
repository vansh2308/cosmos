#include <iostream>
#include <vector>

#include "InterpreterDriver/InterpreterDriver.h"


int main(int argc, char const *argv[]){
    if (argc > 2){
        std::cout << "Usage: ./csm <script.csm> to execute a script or just ./csm to drop into a REPL";
        std::exit(64);
    }

    cosmos::InterpreterDriver interpreter;

    if(argc == 2){
        interpreter.run_script(argv[1]);
    }

    interpreter.run_REPL();
    return 0;
}

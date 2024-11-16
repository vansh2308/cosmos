#include <iostream>
#include <vector>

#include "InterpreterDriver/InterpreterDriver.h"


int main(int argc, char const *argv[]){
    cosmos::InterpreterDriver interpreter;
    cout << interpreter.run_script("hey") << endl;

    if (argc > 2){
        std::cout << "Usage: ./csm <script.csm> to execute a script or just ./csm to drop into a REPL";
        std::exit(64);
    }
    
    if(argc == 2){
        // WIP: Script 
    }

    // WIP: REPL

    return 0;
}

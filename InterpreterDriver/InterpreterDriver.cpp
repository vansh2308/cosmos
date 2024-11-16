
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


namespace cosmos{
    auto InterpreterDriver::run_script(const char* const scriptFile) -> int {
        cout << scriptFile << endl;
        return 5;
    }


    



    InterpreterDriver::InterpreterDriver() {}

} //namespace cosmos


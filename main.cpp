#include <iostream>

using namespace std;

int main(int argc, char const *argv[]){
    if (argc > 2){
        cout << "Usage: ./csm <script.csm> to execute a script or just ./csm to drop into a REPL";
        exit(64);
    }

    if(argc == 2){
        // WIP: Script 
    }

    // WIP: REPL

    return 0;
}
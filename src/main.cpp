#include <fstream>
#include <iostream>
#include <string>
#include "../include/Token.hpp"

std::string read_file(std::string_view filename){
    std::ifstream file(filename.data(), std::ios::ate);
    if(!file){
        std::cerr << "Failed to open file " << filename.data() << "\n";
        std::exit(74); // IO error
    }

    std::string file_contents;
    file_contents.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(file_contents.data(), file_contents.size());
    file.close();

    return file_contents;
}


void run(const std::string& source){
    // WIP 
}


void init_file(const std::string& filename){
    std::string file_contents = read_file(filename);
    run(file_contents);

    // WIP: Add compile / runtime errors
}



void run_prompt(){
    while(true){
        std::cout << "> ";
        std::string line;
        if(!std::getline(std::cin, line)){
            return;
        }

        run(line);

        // WIP: Add compile / runtime errors

        std::cout << "\n";

    }
}



int main(int argc, char* argv[]){
    if(argc > 2){
        std::exit(64);
    } else if (argc == 2){
        init_file(argv[1]);
    } else {
        run_prompt();
    }

    return 0;
}
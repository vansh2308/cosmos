cmake_minimum_required(VERSION 3.12)
project(HelloWorld)

set(CMAKE_CXX_STANDARD 17)

#Add the library
add_library(ASTNodes AST/nodeTypes.cpp AST/nodeTypes.h)
add_library(prettyPrinter AST/prettyPrinter.cpp AST/prettyPrinter.h)
add_library(evaluator Evaluator/environment.cpp Evaluator/environment.h Evaluator/evaluator.cpp Evaluator/evaluator.h Evaluator/objects.cpp Evaluator/objects.h)
add_library(interpreterDriver interpreterDriver/InterpreterDriver.cpp interpreterDriver/InterpreterDriver.h)
add_library(parser Parser/parser.cpp Parser/parser.h)
add_library(scanner Scanner/scanner.cpp Scanner/scanner.h)
add_library(types Types/literal.cpp Types/literal.h Types/token.cpp Types/token.h Types/uncopyable.h)

#build the executable
add_executable(cosmos main.cpp)


#Link the executable with the library
target_link_libraries(cosmos ASTNodes prettyPrinter evaluator interpreterDriver parser scanner types)
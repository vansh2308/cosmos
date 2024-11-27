cmake_minimum_required(VERSION 3.16)
project(cosmos
    LANGUAGES CXX 
    VERSION 0.1.0 
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)

add_subdirectory(src)

if(CMAKE_PROJECT_NAME STREQUAL cosmos)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()


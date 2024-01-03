cmake_minimum_required(VERSION 3.14.0)

add_definitions(
        -D_PROJECT_NAME="Sunbird"
        -D_PROJECT_AUTHOR="jamadaha"
        -D_PROJECT_VERSION="dev"
)

project(
    ${_PROJECT_NAME}
    VERSION ${_PROJECT_VERSION}
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -march=native -flto)

include(chess/CMakeLists.txt)
include(engine/CMakeLists.txt)
include(bin/CMakeLists.txt)

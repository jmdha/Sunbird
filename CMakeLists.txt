cmake_minimum_required(VERSION 3.14.0)

add_definitions(
        -D_PROJECT_NAME="Sunbird"
        -D_PROJECT_AUTHOR="jamadaha"
        -D_PROJECT_VERSION="1.2.2"
)

project(
    ${_PROJECT_NAME}
    VERSION ${_PROJECT_VERSION}
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

include(chess/CMakeLists.txt)
include(engine/CMakeLists.txt)
include(bin/CMakeLists.txt)

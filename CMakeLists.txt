cmake_minimum_required(VERSION 3.14.0)
add_definitions(
    -D_PROJECT_NAME="Sunbird"
    -D_PROJECT_VERSION="1.1.2"
    -D_PROJECT_AUTHOR="jamadaha"
    )
project(${_PROJECT_NAME} VERSION ${_PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 20)

if (IGNORE_ENGINE)
    include(chess/CMakeLists.txt)
else ()
    include(engine/CMakeLists.txt)
endif ()


cmake_minimum_required(VERSION 3.30)
project(SectorParser)

set(CMAKE_CXX_STANDARD 20)

find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(SectorParser main.cpp)
target_link_libraries(SectorParser PRIVATE raylib nlohmann_json::nlohmann_json)

cmake_minimum_required (VERSION 2.6)
project(hashcode-2016)

SET(CMAKE_BUILD_TYPE Debug)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(SOURCE_DIR ${PROJECT_BINARY_DIR}/src/main/berlin/nadolski/hashcode)


set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

include_directories(${PROJECT_BINARY_DIR}/include)

install(FILES ${SOURCE_DIR}/*.h
        DESTINATION include/berlin/nadolski/hashcode
        CONFIGURATIONS Release Debug)

add_library(hashcode-2016_strategies
    ${SOURCE_DIR}/Command.cpp
    ${SOURCE_DIR}/Drone.cpp
    ${SOURCE_DIR}/Order.cpp
    ${SOURCE_DIR}/Problem.cpp
    ${SOURCE_DIR}/SimpleStrategy.cpp
    ${SOURCE_DIR}/Strategy.cpp
    ${SOURCE_DIR}/Warehouse.cpp)

install(TARGETS hashcode-2016_strategies
        ARCHIVE DESTINATION lib
        CONFIGURATIONS Release Debug)

add_executable(hashcode-2016 ${SOURCE_DIR}/hashcode.cpp)
target_link_libraries(hashcode-2016 hashcode-2016_strategies)

add_dependencies(hashcode-2016 hashcode-2016_strategies)

install(TARGETS hashcode-2016
        RUNTIME DESTINATION bin
        CONFIGURATIONS Release Debug)
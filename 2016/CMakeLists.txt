cmake_minimum_required (VERSION 2.6)
project(hashcode-2016)

SET(CMAKE_BUILD_TYPE Release)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(SOURCE_DIR ${PROJECT_BINARY_DIR}/src/main/berlin/nadolski/hashcode)


set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_ASSERTS "-O3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

include_directories(${PROJECT_BINARY_DIR}/include)

install(FILES ${SOURCE_DIR}/*.h
        DESTINATION include/berlin/nadolski/hashcode
        CONFIGURATIONS Release Debug)

add_library(hashcode2016
    ${SOURCE_DIR}/Command.cpp
    ${SOURCE_DIR}/ProblemState.cpp
    ${SOURCE_DIR}/SimpleStrategy.cpp
    ${SOURCE_DIR}/MyStrategy.cpp
    ${SOURCE_DIR}/NearestWarehouse.cpp)

install(TARGETS hashcode2016
        ARCHIVE DESTINATION lib
        CONFIGURATIONS Release Debug)

add_executable(simulation ${SOURCE_DIR}/simulation.cpp ${SOURCE_DIR}/Simulation.cpp)
target_link_libraries(simulation hashcode2016)
add_dependencies(simulation hashcode2016)

add_executable(simple_strategy ${SOURCE_DIR}/simple_strategy.cpp)
target_link_libraries(simple_strategy hashcode2016)
add_dependencies(simple_strategy hashcode2016)

add_executable(my_strategy ${SOURCE_DIR}/my_strategy.cpp)
target_link_libraries(my_strategy hashcode2016)
add_dependencies(my_strategy hashcode2016)

add_executable(nearest_warehouse ${SOURCE_DIR}/nearest_warehouse.cpp)
target_link_libraries(nearest_warehouse hashcode2016)
add_dependencies(nearest_warehouse hashcode2016)


install(TARGETS simulation
        RUNTIME DESTINATION bin
        CONFIGURATIONS Release Debug)

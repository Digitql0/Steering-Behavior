cmake_minimum_required(VERSION 3.10)
project(SteeringBehavior LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)

find_package(raylib REQUIRED)

# Collect sources
file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS
    src/*.cpp
    src/*.c
)
list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# Build as a static library
add_library(SteeringBehavior STATIC ${LIB_SOURCES})
target_include_directories(SteeringBehavior PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(SteeringBehavior raylib)

set(APP_SOURCES src/main.cpp)

# Build as an Executable
add_executable(SteeringBehaviorApp ${APP_SOURCES})
target_link_libraries(SteeringBehaviorApp PRIVATE SteeringBehavior)

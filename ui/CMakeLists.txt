cmake_minimum_required(VERSION 2.8)
project(objexport-viewer)

set(EXECUTABLE_NAME viewer)

# c++14 flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++1y")

# add OGLW library
add_subdirectory(${PROJECT_SOURCE_DIR}/oglw/oglw)

# include OGLW headers
include_directories(${OGLW_INCLUDE_DIRS})

set(OGLW_BUILD_WITH_GAMMA OFF CACHE BOOL "Build the OGLW with Gamma library")
add_definitions(-DOGLW_DEBUG)

# find resources
file(GLOB_RECURSE RESOURCES ${PROJECT_SOURCE_DIR}/resources/*)

# create an executable bundled with resources (OS X)
add_executable(${EXECUTABLE_NAME} MACOSX_BUNDLE main.cpp ${RESOURCES})
target_link_libraries(${EXECUTABLE_NAME} OGLW ${OGLW_LIBRARIES})
set_target_properties(${EXECUTABLE_NAME} PROPERTIES RESOURCE "${RESOURCES}")


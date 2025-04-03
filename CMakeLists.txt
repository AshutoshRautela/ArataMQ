include("cmake/vcpkg.cmake")

cmake_minimum_required(VERSION 3.31)
project(ArataMQ)

# Add src directory to include path
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Find all source files in src directory
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add main.cpp from root directory
set(MAIN_SOURCE "main.cpp")

# Check if main.cpp exists
if(NOT EXISTS ${MAIN_SOURCE})
    message(FATAL_ERROR "main.cpp not found in root directory")
endif()

add_executable(${PROJECT_NAME} ${MAIN_SOURCE} ${SOURCES})

# Find nlohmann-json
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

# Find spdlog
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
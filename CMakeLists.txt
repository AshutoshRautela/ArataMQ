include("cmake/vcpkg.cmake")

cmake_minimum_required(VERSION 3.31)
project(ArataMQ VERSION 0.1.0)

# Enable tests
enable_testing()

# Add src directory to include path
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Find all source files in src directory
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add main.cpp from root directory
set(MAIN_SOURCE "main.cpp")
set(MAIN_SOURCE_PRODUCER "processes/producer_process.cpp")
set(MAIN_SOURCE_CONSUMER "processes/consumer_process.cpp")

# Check if main.cpp exists
if(NOT EXISTS ${MAIN_SOURCE})
    message(FATAL_ERROR "main.cpp not found in root directory")
endif()

add_executable(${PROJECT_NAME} ${MAIN_SOURCE} ${SOURCES})
add_executable(${PROJECT_NAME}_producer ${MAIN_SOURCE_PRODUCER} ${SOURCES})
add_executable(${PROJECT_NAME}_consumer ${MAIN_SOURCE_CONSUMER} ${SOURCES})

# Find nlohmann-json
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME}_producer PRIVATE nlohmann_json::nlohmann_json)    
target_link_libraries(${PROJECT_NAME}_consumer PRIVATE nlohmann_json::nlohmann_json)

# Find spdlog
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
target_link_libraries(${PROJECT_NAME}_producer PRIVATE spdlog::spdlog)
target_link_libraries(${PROJECT_NAME}_consumer PRIVATE spdlog::spdlog)

# Include gtest.cmake
include("cmake/gtest.cmake")
# Find Google Test
find_package(GTest CONFIG REQUIRED)

# Collect all test source files
file(GLOB_RECURSE TEST_SOURCES 
    "tests/*.cpp"
)

# Collect all source files except main.cpp
file(GLOB_RECURSE LIB_SOURCES 
    "src/core/*.cpp"
    "src/utils/*.cpp"
)

add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES} ${LIB_SOURCES})

# Add include directories
target_include_directories(${PROJECT_NAME}_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME}_tests PRIVATE 
    GTest::gtest 
    GTest::gtest_main 
    GTest::gmock 
    GTest::gmock_main
    nlohmann_json::nlohmann_json
    spdlog::spdlog
)

add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests)


#!/bin/bash

# Function to check if there are new test files
check_for_new_tests() {
    local test_files=$(find tests -name "*.cpp" -type f)
    local build_test_files=$(find build/debug/CMakeFiles/ArataMQ_tests.dir/tests -name "*.cpp.o" -type f 2>/dev/null)
    
    # If no build files exist, we need to clean
    if [ -z "$build_test_files" ]; then
        return 0
    fi
    
    # Compare the lists
    for file in $test_files; do
        local base_name=$(basename "$file" .cpp)
        if ! find build/debug/CMakeFiles/ArataMQ_tests.dir/tests -name "${base_name}.cpp.o" -type f | grep -q .; then
            return 0
        fi
    done
    
    return 1
}

# Check if we need to clean
if check_for_new_tests; then
    echo "New test files detected, cleaning build directory..."
    rm -rf build/debug
    cmake --preset debug
fi

# Build and run tests
cmake --build --preset debug && cd build/debug && ./ArataMQ_tests --gtest_color=yes 
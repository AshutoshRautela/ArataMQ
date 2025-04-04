#!/bin/bash

# Build and run tests
cmake --build --preset debug && cd build/debug && ctest --output-on-failure 
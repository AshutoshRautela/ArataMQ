#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print status messages
print_status() {
    echo -e "${GREEN}[BUILD]${NC} $1"
}

# Function to print error messages
print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to print info messages
print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

# Function to show usage
show_usage() {
    echo "Usage: $0 [options]"
    echo "Options:"
    echo "  -r, --run     Run the debug build after compilation"
    echo "  -h, --help    Show this help message"
    exit 1
}

# Parse command line arguments
RUN_AFTER_BUILD=false

while [[ $# -gt 0 ]]; do
    case $1 in
        -r|--run)
            RUN_AFTER_BUILD=true
            shift
            ;;
        -h|--help)
            show_usage
            ;;
        *)
            print_error "Unknown option: $1"
            show_usage
            ;;
    esac
done

# Clean build directory
print_status "Cleaning build directory..."
rm -rf build/debug

# Configure project
print_status "Configuring project..."
if ! cmake --preset debug; then
    print_error "Configuration failed"
    exit 1
fi

# Build project
print_status "Building project..."
if ! cmake --build --preset debug; then
    print_error "Build failed"
    exit 1
fi

print_status "Build completed successfully!"

# Run the debug build if requested
if [ "$RUN_AFTER_BUILD" = true ]; then
    print_info "Running debug build..."
    ./build/debug/ArataMQ
fi 
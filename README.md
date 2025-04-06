# ArataMQ

ArataMQ is a message queue system implementation that supports different types of exchanges and message routing patterns.

## Project Structure

```
ArataMQ/
├── src/                    # Source code
│   ├── core/              # Core functionality
│   │   ├── broker/        # Broker implementation
│   │   ├── exchange/      # Exchange types (topic, direct)
│   │   ├── message/       # Message handling
│   │   └── queue/         # Queue implementation
│   ├── utils/             # Utility functions
│   └── config/            # Configuration
├── tests/                  # Test files
│   └── unit/              # Unit tests
│       ├── exchange/      # Exchange tests
│       └── queue/         # Queue tests
├── scripts/               # Build and test scripts
└── processes/             # Example producer/consumer processes
```

## Prerequisites

- C++20 compatible compiler
- CMake 3.31 or higher
- vcpkg for dependency management
- pkg-config

## Building the Project

1. Clone the repository:
```bash
git clone <repository-url>
cd ArataMQ
```

2. Build the project:
```bash
./scripts/build.sh
```

## Running Tests

To run all tests:
```bash
./scripts/run_tests.sh
```

The test script automatically detects new test files and handles build cleaning when necessary.

### Current Test Coverage

The project currently has basic test coverage for:
- Queue operations (enqueue, dequeue)
- Topic exchange pattern matching

More test cases will be added to cover:
- Direct exchange functionality
- Message handling
- Broker operations
- Error cases and edge conditions

## Development

### Adding New Tests

1. Create test files in the appropriate directory under `tests/unit/`
2. Follow the existing test structure:
   - Use `TEST_F` for test fixtures
   - Place test classes in the `armq::test` namespace
   - Make test member variables `protected` for access in test methods

### Running Specific Tests

To run specific test suites or tests:
```bash
cd build/debug
./ArataMQ_tests --gtest_filter=TestSuiteName.*  # Run all tests in a suite
./ArataMQ_tests --gtest_filter=TestSuiteName.TestName  # Run specific test
```
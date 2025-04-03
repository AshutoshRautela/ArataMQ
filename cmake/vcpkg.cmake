# vcpkg.cmake
if(DEFINED ENV{VCPKG_ROOT})
    # If VCPKG_ROOT environment variable is set, use it
    set(VCPKG_ROOT $ENV{VCPKG_ROOT})
    message(STATUS "Using vcpkg from environment variable: ${VCPKG_ROOT}")
else()
    # Default paths if environment variable is not set
    if(WIN32)
        set(VCPKG_ROOT "C:/vcpkg" CACHE PATH "Path to vcpkg root")
    elseif(APPLE)
        set(VCPKG_ROOT "$ENV{HOME}/Documents/vcpkg" CACHE PATH "Path to vcpkg root")
    else()
        set(VCPKG_ROOT "/opt/vcpkg" CACHE PATH "Path to vcpkg root")
    endif()
    message(STATUS "Using default vcpkg path: ${VCPKG_ROOT}")
endif()

# Check if vcpkg is already installed
if(NOT EXISTS "${VCPKG_ROOT}/vcpkg")
    message(STATUS "vcpkg not found. Downloading and bootstrapping vcpkg...")

    # Download vcpkg
    set(VCPKG_URL "https://github.com/microsoft/vcpkg/archive/refs/heads/master.zip")
    set(VCPKG_ARCHIVE "${VCPKG_ROOT}/vcpkg.zip")

    file(DOWNLOAD ${VCPKG_URL} ${VCPKG_ARCHIVE} SHOW_PROGRESS)

    # Extract vcpkg
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xzf ${VCPKG_ARCHIVE}
        WORKING_DIRECTORY ${VCPKG_ROOT}
    )

    # Rename the extracted directory to 'vcpkg'
    file(RENAME "${VCPKG_ROOT}/vcpkg-master" "${VCPKG_ROOT}/vcpkg")
    file(REMOVE ${VCPKG_ARCHIVE})

    # Bootstrap vcpkg
    if(WIN32)
        set(BOOTSTRAP_SCRIPT "bootstrap-vcpkg.bat")
    else()
        set(BOOTSTRAP_SCRIPT "bootstrap-vcpkg.sh")
    endif()

    execute_process(
        COMMAND "${VCPKG_ROOT}/${BOOTSTRAP_SCRIPT}"
        WORKING_DIRECTORY ${VCPKG_ROOT}
    )

    message(STATUS "vcpkg has been successfully bootstrapped at ${VCPKG_ROOT}")
else()
    message(STATUS "vcpkg found at ${VCPKG_ROOT}")
endif()

# Set the toolchain file path
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# Check for pkg-config
find_program(PKG_CONFIG_EXECUTABLE pkg-config)
if(NOT PKG_CONFIG_EXECUTABLE)
    message(FATAL_ERROR 
"pkg-config not found! This is required for building ArataMQ.

Please install pkg-config:
  - macOS:    brew install pkg-config
  - Ubuntu:   sudo apt-get install pkg-config
  - Fedora:   sudo dnf install pkgconfig
  - Windows:  vcpkg install pkgconfig

After installing, try building again.
")
endif()
cmake_minimum_required(VERSION 3.16)
project(LeadGenNetwork)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# macOS deployment target (optional but suppresses some linker warnings)
set(CMAKE_OSX_DEPLOYMENT_TARGET "15.4")

# Toolchain paths (for pybind11 and Python)
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/vcpkg/installed/arm64-osx")

# Use pkg-config to locate both libpqxx and libpq
find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)
pkg_check_modules(PQ REQUIRED libpq)

# Find Python3 (Interpreter, Headers, and Embed libs)
find_package(Python3 REQUIRED COMPONENTS Interpreter Development.Module Development.Embed)

# Find pybind11 (from vcpkg toolchain)
find_package(pybind11 CONFIG REQUIRED)

# Include PostgreSQL and Python include paths
include_directories(${PQXX_INCLUDE_DIRS} ${PQ_INCLUDE_DIRS} ${Python3_INCLUDE_DIRS})
link_directories(${PQXX_LIBRARY_DIRS} ${PQ_LIBRARY_DIRS} ${Python3_LIBRARY_DIRS})
add_definitions(${PQXX_CFLAGS_OTHER} ${PQ_CFLAGS_OTHER})

# Add source files
set(SOURCES
    main.cpp
    src/dbConnector.cpp
    src/dbHandler.cpp
    src/simCalculator.cpp
    src/mathFunctions.cpp
    src/jsonConverter.cpp
    src/promptExtractor.cpp
)

# Build main executable
add_executable(leadgen-network ${SOURCES})
target_link_libraries(leadgen-network
  PRIVATE
    pybind11::embed
    ${Python3_LIBRARIES}
    ${PQXX_LIBRARIES}
    ${PQ_LIBRARIES}
)

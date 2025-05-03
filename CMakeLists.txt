cmake_minimum_required(VERSION 3.16)
project(LeadGenNetwork)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# macOS deployment target
set(CMAKE_OSX_DEPLOYMENT_TARGET "15.4")

# Toolchain paths (for pybind11 and Python)
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/vcpkg/installed/arm64-osx")

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)
pkg_check_modules(PQ REQUIRED libpq)

# Find Python3
find_package(Python3 REQUIRED COMPONENTS Interpreter Development.Module Development.Embed)

# Find pybind11
find_package(pybind11 CONFIG REQUIRED)

# Find RapidJSON
find_package(RapidJSON REQUIRED)

# Find Boost (required by Crow)
find_package(Boost REQUIRED COMPONENTS system)

# Find OpenSSL (required by Crow)
find_package(OpenSSL REQUIRED)

# Add Crow via FetchContent
include(FetchContent)
FetchContent_Declare(
    Crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow
    GIT_TAG master  # Using master instead of v1.0+5
    GIT_SHALLOW TRUE
)
set(CROW_BUILD_EXAMPLES OFF CACHE INTERNAL "")
set(CROW_BUILD_TESTS OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(Crow)

# Include directories
include_directories(
    ${PQXX_INCLUDE_DIRS}
    ${PQ_INCLUDE_DIRS}
    ${Python3_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
    ${RAPIDJSON_INCLUDE_DIRS}
    ${Crow_SOURCE_DIR}/include  # Add Crow's include directory explicitly
)

# Link directories
link_directories(
    ${PQXX_LIBRARY_DIRS}
    ${PQ_LIBRARY_DIRS}
    ${Python3_LIBRARY_DIRS}
)

# Add definitions
add_definitions(${PQXX_CFLAGS_OTHER} ${PQ_CFLAGS_OTHER})

# Add source files
set(SOURCES
    src/server.cpp
    src/dbConnector.cpp
    src/dbHandler.cpp
    src/simCalculator.cpp
    src/mathFunctions.cpp
    src/jsonConverter.cpp
)

# Create server executable
add_executable(server ${SOURCES})

# Link libraries
target_link_libraries(server
    PRIVATE
    Crow::Crow
    pybind11::embed
    ${Python3_LIBRARIES}
    ${PQXX_LIBRARIES}
    ${PQ_LIBRARIES}
    ${Boost_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
)

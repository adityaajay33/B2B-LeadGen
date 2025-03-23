cmake_minimum_required(VERSION 3.16)
project(LeadGenNetwork)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use pkg-config to locate both libpqxx and libpq
find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)
pkg_check_modules(PQ REQUIRED libpq)

# Include directories
include_directories(${PQXX_INCLUDE_DIRS} ${PQ_INCLUDE_DIRS})
link_directories(${PQXX_LIBRARY_DIRS} ${PQ_LIBRARY_DIRS})
add_definitions(${PQXX_CFLAGS_OTHER} ${PQ_CFLAGS_OTHER})

# Add source files explicitly
set(SOURCES
    main.cpp
    src/dbConnector.cpp
    src/dbHandler.cpp
    src/simCalculator.cpp
    src/mathFunctions.cpp
    src/jsonConverter.cpp
)
add_compile_options(-Wno-unknown-attributes)

# Create executable
add_executable(leadgen-network ${SOURCES})

# Link with libpqxx, libpq, and ensure DBConnector and DBHandler are linked
target_link_libraries(leadgen-network PRIVATE ${PQXX_LIBRARIES} ${PQ_LIBRARIES})

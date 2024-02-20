cmake_minimum_required(VERSION 3.22)
# set c++ standard
set(CMAKE_CXX_STANDARD 17)

project(CLLVM)

include(FetchContent)
find_package(LLVM REQUIRED CONFIG)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "-g")
endif()

set(CMAKE_CXX_FLAGS "-Wno-trigraphs")

include_directories(include)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Find all the source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c" "include/*.h" "include/*.hpp")
list(FILTER SOURCES EXCLUDE REGEX ".*main.cpp")
list(FILTER SOURCES EXCLUDE REGEX ".*test/.*")

# Add the executable
add_executable(CLLVM src/main.cpp ${SOURCES})

# Link against LLVM libraries
target_link_libraries(CLLVM LLVM)

# Add the test executable

file(GLOB_RECURSE TEST_SOURCES "src/*.cpp" "src/*.c" "include/*.h" "include/*.hpp" "test/*.cpp" "test/*.c" "test/*.h" "test/*.hpp")
list(FILTER TEST_SOURCES EXCLUDE REGEX ".*main.cpp")

# dont include files used
list(FILTER TEST_SOURCES EXCLUDE REGEX ".*/test_files/*")

# if there arent any test files, remove the test executable
if (TEST_SOURCES STREQUAL "")
  return()
endif()

add_executable(test ${TEST_SOURCES})

# Link against LLVM libraries
target_link_libraries(test LLVM)

# Add the gtest library
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Link against gtest
target_link_libraries(test gtest_main)

add_test(NAME test COMMAND test)
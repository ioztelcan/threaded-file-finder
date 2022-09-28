cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)

project(
  threaded_file_finder
  VERSION 0.0.1
  DESCRIPTION ""
  HOMEPAGE_URL ""
  LANGUAGES CXX)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -pedantic")

# Sanitizer flags
set(ENABLE_SANITIZERS ON CACHE BOOL "Enable Sanitizers")
if (ENABLE_SANITIZERS)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer")
  set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer")
endif()

set(ENABLE_CLANG_TIDY ON CACHE BOOL "Add clang-tidy to builds")
if (ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY NAMES "clang-tidy")
    if (CLANG_TIDY)
        message(STATUS "clang-tidy found: ${CLANG_TIDY}")
        set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY})
    else()
        message(AUTHOR_WARNING "clang-tidy not found!")
        set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE) # delete it
    endif()
endif()

add_subdirectory(src)

# Enable google test
option(ENABLE_TESTING "Enable the tests" ON)
if(ENABLE_TESTING)
  enable_testing()
  add_custom_target(build_and_test ${CMAKE_CTEST_COMMAND} -V)
  add_subdirectory(test)
endif()

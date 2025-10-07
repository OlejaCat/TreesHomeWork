cmake_minimum_required(VERSION 3.14)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(trees) 

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_DEBUG "Enable debug build" OFF)
option(ENABLE_CLANG_TIDY "Enable clang-tidy checks" OFF)
option(BUILD_TESTS "Build tests" ON)


if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY clang-tidy)
    if(CLANG_TIDY)
        set(CMAKE_CXX_CLANG_TIDY 
            ${CLANG_TIDY}
        )
        message(STATUS "Clang-tidy enabled: ${CLANG_TIDY}")
    else()
        message(WARNING "Clang-tidy not found!")
    endif()
endif()

include(cmake/compiler_flags.cmake)

add_executable(trees)

target_include_directories(trees
    PRIVATE
        src/tree
)

target_sources(trees
    PRIVATE
        src/app/main.cpp
)

target_link_libraries(trees project_compiler_flags)

if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL "https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip"
    )
    
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    
    add_executable(tests tests/unit_test_avl.cpp tests/test_solution.cpp)
    target_include_directories(tests PRIVATE src/tree common/)
    if (ENABLE_DEBUG)
        target_link_libraries(tests GTest::gtest_main)
    else()
        target_link_libraries(tests GTest::gtest_main project_compiler_flags)
    endif()

    include(GoogleTest)
    gtest_discover_tests(tests)
endif()

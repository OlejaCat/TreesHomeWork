cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(trees) 

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_DEBUG "Enable debug build" ON)
option(ENABLE_CLANG_TIDY "Enable clang-tidy checks" OFF)
option(ENABLE_CLANG_FORMAT "Enable clang-format checks" OFF)
option(BUILD_TESTS "Build tests" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)


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
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    
    add_executable(tests tests/unit_test_avl.cpp tests/test_solution.cpp)
    target_include_directories(tests PRIVATE src/tree)
    target_link_libraries(tests GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(tests)
endif()

if(ENABLE_CLANG_FORMAT)
    find_program(CLANG_FORMAT clang-format)
    if(CLANG_FORMAT)
        file(GLOB_RECURSE ALL_SOURCE_FILES src/*.cpp tests/*.cpp examples/*.cpp)
        file(GLOB_RECURSE ALL_HEADER_FILES src/*.hpp include/*.hpp)
        
        add_custom_target(format
            COMMAND ${CLANG_FORMAT} -i --style=file ${ALL_SOURCE_FILES} ${ALL_HEADER_FILES}
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMENT "Formatting all source and header files"
        )
    endif()
endif()

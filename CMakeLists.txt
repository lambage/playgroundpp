cmake_minimum_required(VERSION 3.24)

project(
    playgroundpp
    VERSION 1.0
    LANGUAGES CXX
)

if (ADDRESS_SANITIZER)
    message(STATUS "Instrumenting code for address sanitization")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libasan")
    endif()
endif(ADDRESS_SANITIZER)

if (THREAD_SANITIZER)
    message(STATUS "Instrumenting code for thread sanitization")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=thread -gcolumn-info")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=thread")
endif(THREAD_SANITIZER)

if (UB_SANITIZER)
    message(STATUS "Instrumenting code for undefined behavior sanitization")
    set(UBSAN_FLAGS "-fsanitize=undefined -fsanitize=implicit-conversion -fsanitize=integer -fsanitize-trap=integer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer ${UBSAN_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fno-omit-frame-pointer ${UBSAN_FLAGS}")
endif(UB_SANITIZER)

find_package( Boost REQUIRED COMPONENTS system )
find_package( Threads REQUIRED )

set(CLI11_PRECOMPILED ON)
add_subdirectory("third_party/CLI11")
add_subdirectory(src)

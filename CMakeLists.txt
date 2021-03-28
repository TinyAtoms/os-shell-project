cmake_minimum_required(VERSION 3.15)
project(OS_PROJ C) # change these to C instead of CXX

set(CMAKE_C_STANDARD 99)

add_executable(untitled main.c)
#set(CMAKE_CXX_FLAGS "-O3 -flto -fuse-linker-plugin -march=native")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CLANG_TIDY_CXX -checks=-*,
    bugprone-*,
    cert-*,
    clang-analyzer-*,
    concurrency-*,
    cppcoreguidelines-*,
    hicpp-*,
    llvm-*,
    misc-*,
    modernize-*,
    mpi-*,
    openmp-*,
    performance-*,
    portability-*,
    readability-*,
)
set(CLANG_TIDY_SUPPRESS_CXX
    misc-*,
)

add_executable(osshell
        main.c
       helpers.h
        )
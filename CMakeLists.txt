cmake_minimum_required(VERSION 3.30)
project(CPP_GD2_CA3_Mykola_Anisimov_BUG)

set(CMAKE_CXX_STANDARD 20)

add_executable(CPP_GD2_CA3_Mykola_Anisimov_BUG src/main.cpp
        src/Position.cpp
        src/Position.h
        src/Crawler.cpp
        src/Crawler.h
        src/Board.cpp
        src/Board.h
        src/BUG.h
        src/BUG.cpp
        src/BUG.h
        src/HOPPER.cpp
        src/HOPPER.h
        src/ROLLER.cpp
        src/ROLLER.h)

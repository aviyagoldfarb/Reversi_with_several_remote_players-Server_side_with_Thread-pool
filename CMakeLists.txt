cmake_minimum_required(VERSION 3.8)
project(ex7_server)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS -pthread)

set(SOURCE_FILES main.cpp Server.cpp Server.h Point.cpp Point.h Command.h CommandsManager.cpp CommandsManager.h ListGamesCommand.cpp ListGamesCommand.h StartCommand.cpp StartCommand.h JoinCommand.cpp JoinCommand.h Task.cpp Task.h ThreadPool.cpp ThreadPool.h)
add_executable(ex7_server ${SOURCE_FILES})
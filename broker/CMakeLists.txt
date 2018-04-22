cmake_minimum_required(VERSION 3.10)
project(broker C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DLOG_USE_COLOR")


add_executable(broker client/client.h common/log.c common/log.h common/message.h)
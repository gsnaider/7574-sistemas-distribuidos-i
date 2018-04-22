cmake_minimum_required(VERSION 3.10)
project(broker C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DLOG_USE_COLOR")


add_executable(broker client/client.h common/log/log.c common/log/log.h common/message.h client/client.c common/ipc/msg_queue.c common/ipc/msg_queue.h common/ipc/resources.h common/ipc/shm.c common/ipc/shm.h common/ipc/semaphore.c common/ipc/semaphore.h)
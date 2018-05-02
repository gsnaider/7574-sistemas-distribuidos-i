cmake_minimum_required(VERSION 3.10)
project(broker C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DLOG_USE_COLOR")

set(DEPENDENCIES
        client/broker.h
        common/log/log.c
        common/log/log.h
        common/message.h
        client/broker.c
        common/ipc/msg_queue.c
        common/ipc/msg_queue.h
        common/ipc/resources.h
        common/ipc/shm.c
        common/ipc/shm.h
        common/ipc/semaphore.c
        common/ipc/semaphore.h
        client/broker_handler.h
        common/ipc/socket.c
        common/ipc/socket.h
        common/ipc/sig.c
        common/ipc/sig.h)

add_executable(broker_resp_handler client/broker_resp_handler.c ${DEPENDENCIES})
add_executable(broker_req_handler client/broker_req_handler.c ${DEPENDENCIES})
add_executable(server server/server.c server/server.h ${DEPENDENCIES})
add_executable(req_handler server/req_handler.c ${DEPENDENCIES})
add_executable(resp_handler server/resp_handler.c ${DEPENDENCIES})
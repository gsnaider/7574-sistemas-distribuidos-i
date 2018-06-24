cmake_minimum_required(VERSION 3.10)
project(broker C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DLOG_USE_COLOR")

set(CLIENT_DEPENDENCIES
        client/broker.h
        client/broker.c
        client/broker_id.h
        client/broker_id.c
        client/broker_handler.h)

set(SERVER_DEPENDENCIES
        server/global_id.h
        server/global_id.c
        server/db-connection.h
        server/db-connection.c
        server/server.h)

set(DB_DEPENDENCIES
        db-server/db_server.h
        db-server/db.h
        db-server/db.c
        db-server/db_message.h)

set(COMMON_DEPENDENCIES
        client/broker.h
        common/log/log.c
        common/log/log.h
        common/message.h
        client/broker.c
        client/broker_id.h
        client/broker_id.c
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
        common/ipc/sig.h
        common/ds/data-structures/list.h
        common/ds/implementation/list.c
        common/ds/data-structures/vector.h
        common/ds/implementation/vector.c)

add_executable(broker_resp_handler client/broker_resp_handler.c ${COMMON_DEPENDENCIES} ${CLIENT_DEPENDENCIES})
add_executable(broker_req_handler client/broker_req_handler.c ${COMMON_DEPENDENCIES} ${CLIENT_DEPENDENCIES})
add_executable(client client/client.c ${COMMON_DEPENDENCIES} ${CLIENT_DEPENDENCIES})

add_executable(server server/server.c server/server.h ${COMMON_DEPENDENCIES} ${SERVER_DEPENDENCIES})
add_executable(req_handler server/req_handler.c ${COMMON_DEPENDENCIES} ${SERVER_DEPENDENCIES})
add_executable(resp_handler server/resp_handler.c ${COMMON_DEPENDENCIES} ${SERVER_DEPENDENCIES})
add_executable(worker server/worker.c ${COMMON_DEPENDENCIES} ${SERVER_DEPENDENCIES})
add_executable(ring_connection server/ring_connection.c ${COMMON_DEPENDENCIES} ${SERVER_DEPENDENCIES})

add_executable(db_server db-server/db_server.c ${COMMON_DEPENDENCIES} ${DB_DEPENDENCIES})
add_executable(db_handler db-server/db_handler.c ${COMMON_DEPENDENCIES} ${DB_DEPENDENCIES})
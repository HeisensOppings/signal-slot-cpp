cmake_minimum_required(VERSION 3.10)
project(sigslot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/core)
set(HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# --- library ---
add_library(sigslot STATIC
    ${SRC_DIR}/event.cpp
    ${SRC_DIR}/task_queue_base.cpp
    ${SRC_DIR}/task_queue_manager.cpp
    ${SRC_DIR}/task_queue_stdlib.cpp
    ${SRC_DIR}/task_queue.cpp
    ${SRC_DIR}/time_utils.cpp
    ${SRC_DIR}/system_time.cpp
    ${SRC_DIR}/warn_current_thread_is_deadlocked.cpp
    ${SRC_DIR}/yield.cpp
    ${SRC_DIR}/yield_policy.cpp
)

target_include_directories(sigslot
    PUBLIC
        ${HEADER_DIR}
    PRIVATE
        ${SRC_DIR}
)

if (WIN32)
    target_compile_definitions(sigslot PRIVATE CORE_WIN CORE_HAVE_THREAD_LOCAL)
    target_link_libraries(sigslot PRIVATE winmm)
    message(STATUS "sigslot using CORE_WIN CORE_HAVE_THREAD_LOCAL")
else()
    target_compile_definitions(sigslot PRIVATE CORE_POSIX CORE_HAVE_THREAD_LOCAL)
    message(STATUS "sigslot using CORE_POSIX CORE_HAVE_THREAD_LOCAL")
endif()

option(SIGSLOT_BUILD_EXAMPLES "Build sigslot example programs" ON)
if(SIGSLOT_BUILD_EXAMPLES)
    add_executable(SigSlot main.cpp)
    target_link_libraries(SigSlot PRIVATE sigslot)
endif()

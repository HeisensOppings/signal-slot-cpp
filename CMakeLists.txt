cmake_minimum_required(VERSION 3.10)
project(SigSlot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/core)
set(HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/)

add_executable(SigSlot
    main.cpp
    core/event.cpp
    core/task_queue_base.cpp
    core/task_queue_manager.cpp
    core/task_queue_stdlib.cpp
    core/task_queue.cpp
    core/time_utils.cpp
    core/system_time.cpp
    core/warn_current_thread_is_deadlocked.cpp
    core/yield.cpp
    core/yield_policy.cpp)

target_include_directories(SigSlot PRIVATE "${SRC_DIR}")
target_include_directories(SigSlot PUBLIC "${HEADER_DIR}")

if (WIN32)
    target_compile_definitions(SigSlot PRIVATE CORE_WIN CORE_HAVE_THREAD_LOCAL)
    target_link_libraries(SigSlot winmm)
    message("SigSlot using CORE_WIN CORE_HAVE_THREAD_LOCAL")
else()
    target_compile_definitions(SigSlot PRIVATE CORE_POSIX CORE_HAVE_THREAD_LOCAL)
    message("SigSlot using CORE_POSIX CORE_HAVE_THREAD_LOCAL")
endif()
cmake_minimum_required(VERSION 3.16)
project(sigslot)

set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/../src/3rd/sigslot/core)
set(HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/../src/3rd/sigslot/)

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

if (WIN32)
    target_compile_definitions(sigslot PRIVATE CORE_WIN CORE_HAVE_THREAD_LOCAL)
    target_link_libraries(sigslot winmm)
    message("sigslot using CORE_WIN CORE_HAVE_THREAD_LOCAL")
else()
    target_compile_definitions(sigslot PRIVATE CORE_POSIX CORE_HAVE_THREAD_LOCAL)
    message("sigslot using CORE_POSIX CORE_HAVE_THREAD_LOCAL")
endif()

target_include_directories(sigslot PRIVATE "${SRC_DIR}")
target_include_directories(sigslot PUBLIC "${HEADER_DIR}")
target_compile_features(sigslot PUBLIC cxx_std_17)
# target_compile_options(sigslot PRIVATE -UNDEBUG) # use assert
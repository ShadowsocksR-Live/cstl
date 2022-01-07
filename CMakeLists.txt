cmake_minimum_required(VERSION 3.0)
project(cstl)

include(CMakeDependentOption)

set(CMAKE_CXX_STANDARD 11)

set(CSTL_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "cstl library root directory" FORCE)

cmake_dependent_option(BUILD_CSTL_TESTING
  "Build both cstl library and testing or build Library only"
  ON "CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR" OFF)

if (UNIX)
add_definitions(-Wall -Werror -ggdb3 -std=c99 -Wextra -pedantic)
add_definitions("-D_DEFAULT_SOURCE -D_GNU_SOURCE")
endif()

set(CSTL_SRC_FILES
    inc/c_algorithms.h
    inc/c_array.h
    inc/c_deque.h
    inc/c_errors.h
    inc/c_iterator.h
    inc/c_stl_lib.h
    inc/c_list.h
    inc/c_map.h
    inc/c_rb.h
    inc/c_set.h
    src/c_algorithms.c
    src/c_array.c
    src/c_deque.c
    src/c_list.c
    src/c_map.c
    src/c_rb.c
    src/c_set.c
    src/c_util.c
)

set(CSTL_TEST_FILES
    test/t_c_algorithms.c
    test/t_c_array.c
    test/t_c_deque.c
    test/t_c_map.c
    test/t_c_rb.c
    test/t_c_set.c
    test/t_c_slist.c
    test/t_clib.c
)

include_directories(inc)

add_library(cstl STATIC ${CSTL_SRC_FILES})

if (BUILD_CSTL_TESTING)
    add_executable(test-cstl ${CSTL_TEST_FILES})
    target_link_libraries(test-cstl cstl)
endif()

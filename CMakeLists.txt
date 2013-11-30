# File CMakeLists.txt author Vladislav Tcendrovskii
# Copyright (c) 2013
#   This source subjected to the Gnu General Public License v3 or later (see LICENSE)
#   All other rights reserved
#   THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY 
#   OF ANY  KIND, EITHER EXPRESSED OR IMPLIED, 
#   INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
#   OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.



cmake_minimum_required (VERSION 2.4) 

project(geodesic)

add_executable(geodesic_client message.cc start.cc client.cc light_source.cc io.cc tensors.cc space.cc runge.cc geod.cc)
add_executable(geodesic_server emit.cc emit.cc server_main.cc message.cc start.cc server.cc light_source.cc io.cc tensors.cc  )

add_library(task SHARED task.cc tensors.cc start.cc )

install(TARGETS geodesic_server geodesic_client task  DESTINATION bin)

add_dependencies(geodesic_client task)
add_dependencies(geodesic_server task)



include_directories(${CMAKE_SOURCE_DIR})

target_link_libraries(geodesic_client -lpthread ${CMAKE_BINARY_DIR}/libtask.so)
target_link_libraries(geodesic_server  ${CMAKE_BINARY_DIR}/libtask.so)



set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -DDEBUG ")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG  ")

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

add_executable(geodesic_client src/message.cc src/start.cc src/client.cc src/light_source.cc src/io.cc src/tensors.cc src/space.cc src/runge.cc src/geod.cc)
add_executable(geodesic_server src/server_main.cc src/message.cc src/start.cc src/server.cc src/light_source.cc src/io.cc src/tensors.cc  )
add_library(task SHARED src/task.cc src/tensors.cc src/start.cc src/emit.cc)

install(TARGETS geodesic_server geodesic_client task  DESTINATION bin)

add_dependencies(geodesic_client task)
add_dependencies(geodesic_server task)



include_directories(${CMAKE_SOURCE_DIR}/include)

target_link_libraries(geodesic_client -lpthread ${CMAKE_BINARY_DIR}/libtask.so)
target_link_libraries(geodesic_server  ${CMAKE_BINARY_DIR}/libtask.so)



set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -DDEBUG ")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG  ")

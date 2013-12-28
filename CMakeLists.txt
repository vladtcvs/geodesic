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

add_executable(geodesic_client src/message.cc  src/client.cc  src/io.cc src/space.cc src/geod.cc src/errors.cc)
add_executable(geodesic_server src/eye.cc src/server_main.cc src/message.cc src/server.cc  src/io.cc  src/errors.cc)
add_library(errors SHARED src/errors.cc)
add_library(tensors SHARED src/tensors.cc src/tensor1.cc src/tensor2.cc src/tensor3.cc)
add_library(task SHARED src/task.cc src/start.cc src/emit.cc)
add_library(runge SHARED src/runge.cc)

install(TARGETS geodesic_server geodesic_client task tensors runge DESTINATION bin)

add_dependencies(tensors errors)
add_dependencies(task tensors errors)
add_dependencies(runge tensors errors)
add_dependencies(geodesic_client task tensors runge errors)
add_dependencies(geodesic_server task tensors errors)



include_directories(${CMAKE_SOURCE_DIR}/include)


target_link_libraries(geodesic_client -lpthread ${CMAKE_BINARY_DIR}/libtask.so  ${CMAKE_BINARY_DIR}/librunge.so ${CMAKE_BINARY_DIR}/libtensors.so ${CMAKE_BINARY_DIR}/liberrors.so)
target_link_libraries(geodesic_server  ${CMAKE_BINARY_DIR}/libtask.so ${CMAKE_BINARY_DIR}/libtensors.so ${CMAKE_BINARY_DIR}/liberrors.so)



set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -DDEBUG ")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG  ")


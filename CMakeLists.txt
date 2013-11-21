
cmake_minimum_required (VERSION 2.6) 

project(geodesic)

add_executable(geodesic_client message.cc start.cc client.cc light_source.cc io.cc tensors.cc space.cc runge.cc geod.cc)
add_executable(geodesic_server server_main.cc message.cc start.cc server.cc light_source.cc io.cc tensors.cc  )

add_library(task SHARED task.cc tensors.cc start.cc )

install(TARGETS geodesic_server geodesic_client task  DESTINATION bin)

add_dependencies(geodesic_client task)
add_dependencies(geodesic_server task)



include_directories(${CMAKE_SOURCE_DIR})

target_link_libraries(geodesic_client -lpthread ${CMAKE_BINARY_DIR}/libtask.so)
target_link_libraries(geodesic_server  ${CMAKE_BINARY_DIR}/libtask.so)



set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -DDEBUG ")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG  ")

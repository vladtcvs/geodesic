
cmake_minimum_required (VERSION 2.6) 

project(geodesic)

add_executable(geodesic message.cc server.cc client.cc light_source.cc io.cc task.cc tensors.cc space.cc runge.cc geod.cc)

install(TARGETS geodesic RUNTIME DESTINATION bin)




include_directories(${CMAKE_SOURCE_DIR})

target_link_libraries(geodesic -lpthread)

set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -DDEBUG -DLINUX")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -DLINUX -march=native")

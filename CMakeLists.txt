
cmake_minimum_required (VERSION 2.6) 

project(geodesic)

add_executable(geodesic io.cc task.cc tensors.cc space.cc runge.cc geod.cc)

install(TARGETS geodesic RUNTIME DESTINATION bin)




include_directories(${CMAKE_SOURCE_DIR} /usr/include/glib-2.0 /usr/lib/x86_64-linux-gnu/glib-2.0/include/)

target_link_libraries(geodesic -lglib-2.0)

set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -DDEBUG -DLINUX")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -DLINUX")
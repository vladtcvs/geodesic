project(geodesic)

add_executable(geodesic tensors.cc space.cc runge.cc geod.cc)

install(TARGETS geodesic RUNTIME DESTINATION bin)

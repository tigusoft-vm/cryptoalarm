file(GLOB_RECURSE SIMPLESOUND *)

add_subdirectory(otshell_utils)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_executable(simplesound ${SIMPLESOUND})
target_link_libraries(otshell_utils ${SFML_LIBRARIES})

file(GLOB_RECURSE SIMPLESOUND *)

add_subdirectory(otshell_utils)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wl,--no-as-needed -pthread -std=c++11")
add_executable(simplesound ${SIMPLESOUND})


target_link_libraries(simplesound otshell_utils boost_system boost_filesystem boost_iostreams boost_program_options sfml-audio sfml-graphics sfml-window sfml-system fftw3)

#-lm -lsfml-graphics -lboost_iostreams -lboost_system -lboost_filesystem -lfftw3 -lsfml-audio -lsfml-window -lsfml-system

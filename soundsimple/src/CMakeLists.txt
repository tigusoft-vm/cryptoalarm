file(GLOB_RECURSE SIMPLESOUND *)

add_subdirectory(otshell_utils)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -pthread --std=c++11")


set(SFML_LIBRARIES_DIR "${CMAKE_SOURCE_DIR}/deps/SFML/lib")
set(SFML_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/deps/SFML/include")

add_executable(simplesound ${SIMPLESOUND})

include_directories(${SFML_INCLUDE_DIR})
link_directories(${SFML_LIBRARIES_DIR})

#target_link_libraries(simplesound otshell_utils boost_system boost_filesystem boost_iostreams boost_program_options ${SFML_LIBRARIES_DIR}  fftw3 crypto cryptopp)
target_link_libraries(simplesound otshell_utils boost_system boost_filesystem boost_iostreams boost_program_options sfml-audio sfml-graphics sfml-window sfml-system fftw3 crypto cryptopp)

#-lm -lsfml-graphics -lboost_iostreams -lboost_system -lboost_filesystem -lfftw3 -lsfml-audio -lsfml-window -lsfml-system

add_subdirectory(chainsign)

MESSAGE( STATUS "executable:  ${CMAKE_CURRENT_BINARY_DIR}/simplesound ")


add_custom_target(run
  COMMAND simplesound -a
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}

)

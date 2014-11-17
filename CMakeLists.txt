cmake_minimum_required(VERSION 2.8.6)

if(SFML_STATIC_LIBRARIES)
	add_definitions(-DSFML_STATIC)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(Boost 1.53 REQUIRED COMPONENTS system filesystem iostreams)
find_package(SFML REQUIRED system window audio)

include_directories(soundsimple/src/otshell_utils)
add_subdirectory(soundsimple/src)


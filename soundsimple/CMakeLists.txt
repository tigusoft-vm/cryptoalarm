cmake_minimum_required(VERSION 2.8.6)


set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(Boost 1.49 REQUIRED COMPONENTS system filesystem iostreams)
set(Boost_LIBRARIES "${Boost_LIBRARIES}")
#find_library(SFML 2.0 REQUIRED COMPONENTS system window audio)
#find_package(SFML 2.0 REQUIRED COMPONENTS system window audio)
find_library(FFTW3_LIBRARIES NAMES fftw3 libfftw3)

option(BUILD_TESTS "Build the unit tests." OFF)



# Detect and add SFML
# set(CMAKE_MODULE_PATH "/home/gab-dev/.local/share/SFML/cmake/Modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
execute_process(COMMAND git "submodule" "update" "--init" "--recursive")


find_package(SFML 2.0 COMPONENTS system window graphics network audio)


if(NOT SFML_FOUND)
	MESSAGE("SFML not found")
	add_subdirectory(deps/SFML)
	set(SFML_INCLUDE_DIR
		"${CMAKE_SOURCE_DIR}/deps/SFML/include/"
	
	) 
	set (SFML_LIBRARIES
 	"${CMAKE_SOURCE_DIR}/build/deps/SFML/lib/"
  )
	MESSAGE(STATUS "Using SFML include dir ${SFML_INCLUDE_DIR}")
endif()




if(BUILD_TESTS)
  include(CTest)
  enable_testing()

  set(GTEST_ROOT "dep/gtest")
  set(GTEST_FOUND ON)
  set(GTEST_INCLUDE_DIRS ${GTEST_ROOT}/include)
  set(GTEST_LIBRARIES gtest)
  set(GTEST_MAIN_LIBRARIES gtest_main)
  set(GTEST_BOTH_LIBRARIES ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
endif()


execute_process(COMMAND git "submodule" "update" "--init" "--recursive")




#add_subdirectory(deps)
add_subdirectory(src)
#add_subdirectory(tests)


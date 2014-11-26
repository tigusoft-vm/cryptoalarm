set(NAME unittests-sound-simple)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wl,--no-as-needed -pthread -std=c++11")

include_directories(
  ../deps/gtest/include
  ../src
)


set(SRC
  tests.cpp
)

add_executable(${NAME} ${SRC})

target_link_libraries(${NAME}
  ${GTEST_BOTH_LIBRARIES}
)

set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY tests)

add_test(${NAME} tests/${NAME} --gtest_output=xml:gtestresults.xml)

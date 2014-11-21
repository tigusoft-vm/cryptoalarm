set(NAME unittests-sound-simple)

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

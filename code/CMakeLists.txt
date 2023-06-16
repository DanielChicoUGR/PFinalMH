cmake_minimum_required(VERSION 3.0)
PROJECT(cec17_Daniel_Chico_Valderas)
cmake_minimum_required(VERSION 3.22)

# set(CMAKE_CXX_COMPILER "clang++")
# set(CMAKE_C_COMPILER "clang")

set(BASE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})


set(C_FLAGS "-O3")
set(CXX_FLAGS "-O3 -std=c++23 -fopenmp")

set(SRC ${BASE_FOLDER}/src)
set(INCLUDE ${BASE_FOLDER}/include)

include_directories(${INCLUDE})

ADD_EXECUTABLE(test ${SRC}/test.cc)
ADD_EXECUTABLE(testrandom ${SRC}/testrandom.cc)
ADD_EXECUTABLE(testsolis ${SRC}/testsolis.cc)
ADD_EXECUTABLE(gwo ${SRC}/gwo_main.cc)


ADD_LIBRARY("cec17_test_func" SHARED ${SRC}/cec17_test_func.c ${SRC}/cec17.c )

add_library(GWO_LIB SHARED ${SRC}/gwo_lib.cpp)

TARGET_LINK_LIBRARIES(test "cec17_test_func")
TARGET_LINK_LIBRARIES(testrandom "cec17_test_func")
TARGET_LINK_LIBRARIES(testsolis "cec17_test_func")
TARGET_LINK_LIBRARIES(gwo "cec17_test_func" "GWO_LIB")


find_package(OpenMP)
if (OpenMP_CXX_FOUND)
    target_link_libraries(GWO_LIB PUBLIC OpenMP::OpenMP_CXX)
endif ()
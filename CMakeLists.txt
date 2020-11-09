cmake_minimum_required(VERSION 3.15)
project(parallel_sender)

set(CMAKE_INSTALL_RPATH "$ORIGIN")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}) 

set(SENDER_COMMON
        client/Client.cc
        util/network_utils.cc
        stat_sender_test.cc
        )

set(SERIALIZERS
        serializer/stat.cc
        )

set(SOURCE_FILES
        ${SENDER_COMMON}
        ${SERIALIZERS}
        sender_test_main.cc
        )

# External libs
set(EXT_HEADER_DIR ${PROJECT_SOURCE_DIR}/include)
set(BOOST_LIB_DIR ${PROJECT_SOURCE_DIR}/lib/boost)
set(GMOCK_LIB_DIR ${PROJECT_SOURCE_DIR}/lib/gmock)
set(GTEST_LIB_DIR ${PROJECT_SOURCE_DIR}/lib/gtest)
find_library(
        LIBGTEST
        NAMES libgtest.so
        HINTS ${GTEST_LIB_DIR} /usr/lib
        REQUIRED
)
find_library(
        LIBGTEST_MAIN
        NAMES libgtest_main.so
        HINTS ${GTEST_LIB_DIR} /usr/lib
        REQUIRED
)
find_library(
        LIBGMOCK
        NAMES libgmock.so
        HINTS ${GMOCK_LIB_DIR} /usr/lib
        REQUIRED
)
find_library(
        LIBGMOCK_MAIN
        NAMES libgmock_main.so
        HINTS ${GMOCK_LIB_DIR} /usr/lib
        REQUIRED
)
link_directories(
        ${BOOST_LIB_DIR}
        ${GMOCK_LIB_DIR}
        ${GTEST_LIB_DIR}
)

set(LIBRARIES
        boost_system
        boost_thread
        pthread
        ${BOOST_LIB_DIR}
        ${LIBGTEST}
        ${LIBGTEST_MAIN}
        )

include_directories(
        ${PROJECT_SOURCE_DIR}
        util
        serializer
        ${EXT_HEADER_DIR}
)


add_library(parallel_sender_libs OBJECT ${SOURCE_FILES})

add_executable(parallel_sender $<TARGET_OBJECTS:parallel_sender_libs>)
target_link_libraries(parallel_sender ${LIBRARIES})

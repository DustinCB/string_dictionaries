cmake_minimum_required(VERSION 3.8)
project(string_dictionaries)

# Location of additional cmake modules
#
set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH}
        ${PROJECT_SOURCE_DIR}/cmake)

# Guard against in-source builds and bad build-type strings
#
include(ConfigSafeGuards)


#Global Setup
set(CMAKE_CXX_STANDARD 14)


#-------------------
# set common include folder for module
#-------------------
set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)

#-------------------
# Module source
#-------------------
#set(PROJECT_LIB_NAME ${PROJECT_NAME_STR}_impl)

include_directories(${COMMON_INCLUDES})
#file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
#add_library(${PROJECT_LIB_NAME} ${SRC_FILES})


set(SOURCE_FILES
        include/string-dictionaries/HashTableWithLinearProbing.h
        include/string-dictionaries/BlindTree.h
        include/string-dictionaries/TernaryTree.h
        include/string-dictionaries/hash_function.h
        src/hash_function.cpp)

add_library(string_dictionaries ${SOURCE_FILES})


#Testing
include(CTest)
set(LIBS string_dictionaries)

find_package(Threads REQUIRED)
set(LIBS ${LIBS} Threads::Threads)

find_package(GTest)
if (GTEST_FOUND)
    set(LIBS ${LIBS} GTest::Main)
else ()

    if (CMAKE_COMPILER_IS_GNUCXX)
        add_definitions(-Wall -ansi -Wno-deprecated -pthread)
    endif ()

    if (MSVC)
        #vc 2012 fix for vararg templates
        set(MSVC_COMPILER_DEFS "-D_VARIADIC_MAX=10")
    endif ()

    add_subdirectory(${EXT_PROJECTS_DIR}/gtest)
    set(LIBS ${LIBS} ${GTEST_LIBS_DIR}/libgtest.a ${GTEST_LIBS_DIR}/libgtest_main.a)
endif ()

include(cmake/internal_utils.cmake)

include_directories(${GTEST_INCLUDE_DIRS})
cxx_test_with_flags(string_dictionaries_test "" "${LIBS}" test/string_dictionaries_test.cpp)



#Benchmark
# If you want to self-test benchmark lib too, turn me ON
#
set(BENCHMARK_ENABLE_TESTING OFF)

# Configure google micro benchmark
#

# c++11 is required
#
if ((CMAKE_CXX_COMPILER_ID MATCHES GNU) OR (CMAKE_CXX_COMPILER_ID MATCHES Clang))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
#    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
endif ()

include(ConfigGBench)


cxx_test_with_flags(performance_test "" "${LIBS};benchmark" test/performance_test.cpp)

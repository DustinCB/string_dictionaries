cmake_minimum_required(VERSION 3.8)
project(string_dictionaries)

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


set(SOURCE_FILES include/string-dictionaries/HashTableWithLinearProbing.h src/HashTableWithLinearProbing.cpp)
add_library(string_dictionaries ${SOURCE_FILES})


#Testing
set(LIBS string_dictionaries)

find_package(Threads REQUIRED)
find_package(GTest)
IF (GTEST_FOUND)
    set(LIBS ${LIBS} GTest::Main)
ELSE()

    if(CMAKE_COMPILER_IS_GNUCXX)
        add_definitions(-Wall -ansi -Wno-deprecated -pthread)
    endif()

    if(MSVC)
        #vc 2012 fix for vararg templates
        set(MSVC_COMPILER_DEFS "-D_VARIADIC_MAX=10")
    endif()

    add_subdirectory(${EXT_PROJECTS_DIR}/gtest)
    set(LIBS ${LIBS} ${GTEST_LIBS_DIR}/libgtest.a ${GTEST_LIBS_DIR}/libgtest_main.a)
ENDIF()

include(cmake/internal_utils.cmake)

include_directories(${GTEST_INCLUDE_DIRS})
cxx_test_with_flags(hash_table_with_linear_probing_test "" "${LIBS}" test/HashTableWithLinearProbingTest.cpp)

cxx_test_with_flags(performance_test "" "${LIBS}" test/performance_test.cpp)
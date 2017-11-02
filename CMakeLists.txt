cmake_minimum_required(VERSION 3.8)
project(string_dictionaries)

#Global Setup
find_package(Threads REQUIRED)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-Wall -ansi -Wno-deprecated -pthread)
endif()

if(MSVC)
    #vc 2012 fix for vararg templates
    set(MSVC_COMPILER_DEFS "-D_VARIADIC_MAX=10")
endif()

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


#set(SOURCE_FILES src/HashTableLinearProbing.cpp include/string-dictionaries/HashTableLinearProbing.h)
#add_library(string_dictionaries ${SOURCE_FILES})


add_subdirectory(${EXT_PROJECTS_DIR}/gtest)
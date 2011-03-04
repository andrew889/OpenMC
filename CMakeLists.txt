cmake_minimum_required(VERSION 2.8)

project(OpenMC CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "-Wall -std=c++0x")
endif()

find_package(Boost 1.35.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
add_subdirectory(src)

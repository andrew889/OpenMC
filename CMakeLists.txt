cmake_minimum_required(VERSION 2.8)

project(OpenMC CXX)

macro(setup_groups SRC)
	foreach(CUR_FILE ${SRC})
		file(RELATIVE_PATH FILE_PATH ${SRCD} ${CUR_FILE})
		get_filename_component(FILE_PATH ${FILE_PATH} PATH)
		string(REPLACE "/" "\\" FILE_PATH \\src\\${FILE_PATH})
		source_group(${FILE_PATH} FILES ${CUR_FILE})
	endforeach(CUR_FILE)
endmacro(setup_groups)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "-Wall -std=c++0x")
endif()

find_package(Boost 1.35.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
add_subdirectory(src)

include_directories(${CMAKE_SOURCE_DIR}/src/server)
include_directories(${CMAKE_SOURCE_DIR}/src/shared)

set(SRCD ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC
	${SRCD}/main.cpp
)

add_executable(OpenMC_server ${SRC})
add_dependencies(OpenMC_client OpenMC_shared)
target_link_libraries(OpenMC_server OpenMC_shared)

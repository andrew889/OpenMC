include_directories(${CMAKE_SOURCE_DIR}/src/shared)

set(SRCD ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC
	${SRCD}/Chunk.hpp
	${SRCD}/Octree.cpp
	${SRCD}/Octree.hpp
	${SRCD}/TerrainGen.cpp
	${SRCD}/TerrainGen.hpp
)
add_subdirectory(math)

setup_groups("${SRC}")

add_library(OpenMC_shared ${SRC})

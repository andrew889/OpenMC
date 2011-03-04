include_directories(${CMAKE_SOURCE_DIR}/src/client)
include_directories(${CMAKE_SOURCE_DIR}/src/shared)

set(SRCD ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC
	${SRCD}/main.cpp
)
add_subdirectory(gl)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SFML 2.0 REQUIRED system window)

include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_PATH})
include_directories(${SFML_INCLUDE_DIR})

add_executable(OpenMC_client ${SRC})
add_dependencies(OpenMC_client OpenMC_shared)
target_link_libraries(OpenMC_client OpenMC_shared)
target_link_libraries(OpenMC_client ${OPENGL_LIBRARIES})
target_link_libraries(OpenMC_client ${GLEW_LIBRARY})
target_link_libraries(OpenMC_client ${SFML_LIBRARIES})
set(SRCD ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC ${SRC}
	${SRCD}/BufferObject.hpp
	${SRCD}/Shader.cpp
	${SRCD}/Shader.hpp
	${SRCD}/ShaderProgram.hpp
	${SRCD}/Texture.hpp
	${SRCD}/VertexArrayObject.hpp
	PARENT_SCOPE
)
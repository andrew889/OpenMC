#ifndef OPENMC_GL_VERTEXARRAYOBJECT
#define OPENMC_GL_VERTEXARRAYOBJECT

#include <GL/glew.h>

#include <boost/noncopyable.hpp>

namespace gl {

class VertexArrayObject : boost::noncopyable
{
public:
	VertexArrayObject();
	~VertexArrayObject();

	void bind() const;

	operator GLuint() const;

private:
	GLuint id;
};

inline VertexArrayObject::VertexArrayObject() 
{
	glGenVertexArrays(1, &id);
}

inline VertexArrayObject::~VertexArrayObject()
{
	glDeleteVertexArrays(1, &id);
}

inline void VertexArrayObject::bind() const
{
	glBindVertexArray(id);
}

inline VertexArrayObject::operator GLuint() const
{
	return id;
}

} // namespace gl

#endif // OPENMC_GL_VERTEXARRAYOBJECT

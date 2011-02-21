#ifndef OPENMC_GL_BUFFEROBJECT
#define OPENMC_GL_BUFFEROBJECT

#include <GL/glew.h>

#include <boost/noncopyable.hpp>

namespace gl {

class BufferObject : boost::noncopyable
{
public:
	BufferObject();
	~BufferObject();

	void bind(GLenum target) const;

	operator GLuint() const;

private:
	GLuint id;
};

inline BufferObject::BufferObject() 
{
	glGenBuffers(1, &id);
}

inline BufferObject::~BufferObject()
{
	glDeleteBuffers(1, &id);
}

inline void BufferObject::bind(GLenum target) const
{
	glBindBuffer(target, id);
}

inline BufferObject::operator GLuint() const
{
	return id;
}

} // namespace gl

#endif // OPENMC_GL_BUFFEROBJECT

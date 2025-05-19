#ifdef NDEBUG
	#define GL_CHECK_ERROR(label)
#else
	#define GL_CHECK_ERROR(label) CheckGLError(label)
#endif

#include <iostream>
#include <glad/glad.h>

void CheckGLError(const std::string& label) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL Error at [" << label << "]: " << err << std::endl;
    }
}

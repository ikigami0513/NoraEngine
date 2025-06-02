#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

// Déclaration de la fonction
void CheckGLError(const std::string& label);

// Macro conditionnelle
#ifndef GL_CHECK_ERROR
    #ifdef NDEBUG
        #define GL_CHECK_ERROR(label)
    #else
        #define GL_CHECK_ERROR(label) CheckGLError(label)
    #endif
#endif

#endif // UTILS_HPP

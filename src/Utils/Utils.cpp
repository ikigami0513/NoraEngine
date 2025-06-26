#include "Utils/Utils.hpp"
#include <iostream>
#include <glad/glad.h>
#include <string> // Assure-toi que <string> est inclus pour std::string

// Fonction utilitaire interne pour obtenir la description textuelle d'un code d'erreur OpenGL
// C'est un mappage manuel des codes d'erreur OpenGL standards
const char* GetGLErrorString(GLenum error) {
    switch (error) {
        case GL_NO_ERROR:                      return "No error";
        case GL_INVALID_ENUM:                  return "Invalid enum";
        case GL_INVALID_VALUE:                 return "Invalid value";
        case GL_INVALID_OPERATION:             return "Invalid operation";
        case GL_STACK_OVERFLOW:                return "Stack overflow";
        case GL_STACK_UNDERFLOW:               return "Stack underflow";
        case GL_OUT_OF_MEMORY:                 return "Out of memory";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "Invalid framebuffer operation";
        case GL_CONTEXT_LOST:                  return "Context lost"; // OpenGL 4.5+ / KHR_robustness
        // Ajoutez d'autres codes d'erreur si vous les rencontrez, par exemple pour les shaders
        // case GL_INVALID_INDEX: return "Invalid index"; // si vous utilisez des indices d'attributs spécifiques
        default:                               return "Unknown error";
    }
}

void CheckGLError(const std::string& label) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        // Maintenant, imprime la description textuelle en plus du code numérique
        std::cerr << "OpenGL Error at [" << label << "]: "
                  << GetGLErrorString(err) << " (" << err << ")" << std::endl;
    }
}

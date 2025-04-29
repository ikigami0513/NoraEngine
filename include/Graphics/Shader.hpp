#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
    public:
        unsigned int ID;

        Shader (const char* vertexPath, const char* fragmentPath, const char* geometryShader = nullptr);

        void Use() const;

        // Uniform setters
        void SetBool  (const std::string& name, bool value)     const;
        void SetInt   (const std::string& name, int value)      const;
        void SetFloat (const std::string& name, float value)    const;
        void SetVec2  (const std::string& name, const glm::vec2& val) const;
        void SetVec3  (const std::string& name, const glm::vec3& val) const;
        void SetVec4  (const std::string& name, const glm::vec4& val) const;
        void SetMat2  (const std::string& name, const glm::mat2& val) const;
        void SetMat3  (const std::string& name, const glm::mat3& val) const;
        void SetMat4  (const std::string& name, const glm::mat4& val) const;

    private:
        int GetLoc(const std::string& name) const;

        static std::string ReadFile(const char* path);
        static unsigned int CompileShader(const char* code, GLenum type);
        static void CheckErrors(GLuint object, const std::string& type);
        static std::string ShaderTypeToStr(GLenum type);
};

#endif
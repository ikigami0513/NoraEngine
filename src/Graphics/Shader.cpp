#include "Graphics/Shader.hpp"

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
    std::string vertexCode = ReadFile(vertexPath);
    std::string fragmentCode = ReadFile(fragmentPath);
    std::string geometryCode;

    if (geometryPath) {
        geometryCode = ReadFile(geometryPath);
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // Compile shaders
    unsigned int vertex = CompileShader(vShaderCode, GL_VERTEX_SHADER);
    unsigned int fragment = CompileShader(fShaderCode, GL_FRAGMENT_SHADER);

    unsigned int geometry = 0;
    if (geometryPath) {
        geometry = CompileShader(geometryCode.c_str(), GL_GEOMETRY_SHADER);
    }

    // Link program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    if (geometryPath) {
        glAttachShader(ID, geometry);
    }
    glLinkProgram(ID);
    CheckErrors(ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometryPath) {
        glDeleteShader(geometry);
    }
}

void Shader::Use() const {
    glUseProgram(ID);
}


// Uniform setters
void Shader::SetBool(const std::string& name, bool value) const { glUniform1i(GetLoc(name), (int)value); }
void Shader::SetInt(const std::string& name, int value) const { glUniform1i(GetLoc(name), value); }
void Shader::SetFloat(const std::string& name, float value) const { glUniform1f(GetLoc(name), value); }
void Shader::SetVec2(const std::string& name, const glm::vec2& val) const { glUniform2fv(GetLoc(name), 1, &val[0]); }
void Shader::SetVec3(const std::string& name, const glm::vec3& val) const { glUniform3fv(GetLoc(name), 1, &val[0]); }
void Shader::SetVec4(const std::string& name, const glm::vec4& val) const { glUniform4fv(GetLoc(name), 1, &val[0]); }
void Shader::SetMat2(const std::string& name, const glm::mat2& mat) const { glUniformMatrix2fv(GetLoc(name), 1, GL_FALSE, &mat[0][0]); }
void Shader::SetMat3(const std::string& name, const glm::mat3& mat) const { glUniformMatrix3fv(GetLoc(name), 1, GL_FALSE, &mat[0][0]); }
void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const { glUniformMatrix4fv(GetLoc(name), 1, GL_FALSE, &mat[0][0]); }

int Shader::GetLoc(const std::string& name) const {
    return glGetUniformLocation(ID, name.c_str());
}

std::string Shader::ReadFile(const char* path) {
    try {
        std::ifstream file(path);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    catch (std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_READ: " << e.what() << std::endl;
        return "";
    }
}

unsigned int Shader::CompileShader(const char* code, GLenum type) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);
    CheckErrors(shader, ShaderTypeToStr(type));
    return shader;
}

void Shader::CheckErrors(GLuint object, const std::string& type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(object, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
        }
    }
    else {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(object, 1024, nullptr, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR: \n" << infoLog << std::endl;
        }
    }
}

std::string Shader::ShaderTypeToStr(GLenum type) {
    switch(type) {
        case GL_VERTEX_SHADER: return "VERTEX";
        case GL_FRAGMENT_SHADER: return "FRAGMENT";
        case GL_GEOMETRY_SHADER: return "GEOMETRY";
        default: return "UNKNOWN";
    }
}

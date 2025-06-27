#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include "Graphics/3D/RenderComponent.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <iostream>

class Skybox : public RenderComponent {
    public:
        Skybox();
        ~Skybox();

        void Start() override;
        void Render(Shader& shader, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;
        std::string ShaderType() override;

        void SetFaces(std::vector<std::string> faces);
        std::vector<std::string> GetFaces() const;

    private:
        unsigned int m_skyboxVAO;
        unsigned int m_skyboxVBO;

        std::vector<std::string> m_faces;
        unsigned int m_cubemapTextureID;

        static const float m_vertices[108];

        static unsigned int LoadCubemapTexture(const std::vector<std::string>& faces);
        void SetupSkyboxMesh();
};

#endif
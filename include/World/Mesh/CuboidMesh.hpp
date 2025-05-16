#ifndef CUBOID_MESH_HPP
#define CUBOID_MESH_HPP

#include "World/Component.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <string>
#include <vector>

class CuboidMesh : public Component {
    public:
        CuboidMesh();
        ~CuboidMesh();

        void Start() override;
        void Render(Shader& shader, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

        void SetTexture(std::shared_ptr<Texture> texture);
        Texture GetTexture();

    private:
        void SetupMesh();
        unsigned int m_VAO = 0;
        unsigned int m_VBO = 0;

        std::shared_ptr<Texture> m_texture;
        static const float s_vertices[180];
};

#endif
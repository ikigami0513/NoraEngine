#ifndef CUBOID_MESH_HPP
#define CUBOID_MESH_HPP

#include "World/Mesh/RenderComponent.hpp"
#include "World/Component.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <string>
#include <vector>

class CuboidMesh : public RenderComponent {
    public:
        CuboidMesh();
        ~CuboidMesh();

        void Start() override;
        void Render(Shader& shader, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

    private:
        void SetupMesh();
        unsigned int m_VAO = 0;
        unsigned int m_VBO = 0;

        static const float s_vertices[180];
};

#endif
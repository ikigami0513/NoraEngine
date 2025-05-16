#ifndef SPHERE_MESH_HPP
#define SPHERE_MESH_HPP

#include "World/Mesh/MeshComponent.hpp"
#include "World/Component.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

class SphereMesh : public MeshComponent {
    public:
        SphereMesh(unsigned int sectorCount = 36, unsigned int stackCount = 18);
        ~SphereMesh();

        void Start() override;
        void Render(Shader& shader, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

    private:
        void GenerateVertices();
        void SetupMesh();

        unsigned int m_sectorCount;
        unsigned int m_stackCount;

        unsigned int m_VAO = 0;
        unsigned int m_VBO = 0;

        std::vector<float> m_vertices;
};

#endif
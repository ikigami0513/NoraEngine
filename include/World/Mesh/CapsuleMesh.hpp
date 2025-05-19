#ifndef CAPSULE_MESH_HPP
#define CAPSULE_MESH_HPP

#include <memory>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "World/Mesh/RenderComponent.hpp"
#include "World/Component.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"

class CapsuleMesh : public RenderComponent {
    public:
        CapsuleMesh(float radius = 0.5f, float cylinderHeight = 1.0f, unsigned int sectorCount = 36, unsigned int hemisphereStacks = 18, unsigned int cylinderStacks = 10);
        ~CapsuleMesh();

        void Start() override;
        void Render(Shader& shader, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;

    private:
        void GenerateVertices();
        void SetupMesh();

        float m_radius;
        float m_cylinderHeight;
        unsigned int m_sectorCount;
        unsigned int m_hemisphereStacks; // Stacks pour chaque demi-sphère
        unsigned int m_cylinderStacks; // Stacks pour la partie cylindrique

        unsigned int m_VAO;
        unsigned int m_VBO;

        std::vector<float> m_vertices;
        static const float PI;
};

#endif
#ifndef MESH_HPP
#define MESH_HPP

#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"

#define MAX_BONE_INFLUENCE 4

struct MeshVertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;

    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];

    // weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct MeshTexture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
    public:
        // mesh data
        std::vector<MeshVertex> m_Vertices;
        std::vector<unsigned int> m_Indices;
        std::vector<Texture> m_Textures;
        unsigned int m_VAO;

        Mesh(std::vector<MeshVertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        void Draw(Shader& shader);

    private:
        // render data
        unsigned int m_VBO, m_EBO;

        // initializes all the buffer objects/arrays
        void SetupMesh();
};
 
#endif
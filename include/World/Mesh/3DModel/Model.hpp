#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "World/Mesh/RenderComponent.hpp"
#include "World/Mesh/3DModel/Mesh.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"

class Model : public RenderComponent {
    public:
        // model data
        std::vector<Texture> textures_loaded; // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
        std::vector<Mesh> meshes;
        std::string directory;
        bool gammaCorrection;
        std::string path;

        Model(std::string path_ = "");
        ~Model();

        void Start() override;
        void Render(Shader& shader, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;
        std::string ShaderType();

    private:
        // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
        void LoadModel();

        // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        void ProcessNode(aiNode* node, const aiScene* scene);

        Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

        // checks all material textures of a given type and loads the textures if they're not loaded yet.
        // the required info is returned as a Texture struct.
        std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif
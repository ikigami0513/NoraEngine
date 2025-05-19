#include "World/Mesh/CapsuleMesh.hpp"
#include "World/Entity.hpp"
#include "Graphics/Shader.hpp"
#include <cmath>
#include <iostream>

const float CapsuleMesh::PI = 3.14159265359f;

CapsuleMesh::CapsuleMesh(
    float radius, float cylinderHeight, 
    unsigned int sectorCount, unsigned int hemisphereStacks, 
    unsigned int cylinderStacks
) : 
m_radius(radius), m_cylinderHeight(cylinderHeight), 
m_sectorCount(sectorCount), m_hemisphereStacks(hemisphereStacks), 
m_cylinderStacks(cylinderStacks) {
    // S'assurer que les comptes ne sont pas nuls pour éviter la division par zéro
    if (m_sectorCount == 0) m_sectorCount = 1;
    if (m_hemisphereStacks == 0) m_hemisphereStacks = 1;
    if (m_cylinderStacks == 0) m_cylinderStacks = 1;
}

CapsuleMesh::~CapsuleMesh() {
    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
    }
    if (m_VBO != 0) {
        glDeleteBuffers(1, &m_VBO);
    }
}

void CapsuleMesh::GenerateVertices() {
    m_vertices.clear();

    float x, y, z, u, v;
    float sectorStep = 2.0f * PI / m_sectorCount;
    float stackStep; // Sera différent pour les hémisphères et le cylindre

    // Coordonnées de texture:
    // On va mapper les V de la texture comme suit (approximativement):
    // 0.00 - 0.25 : Demi-sphère inférieure
    // 0.25 - 0.75 : Cylindre
    // 0.75 - 1.00 : Demi-sphère supérieure

    // 1. Demi-sphère supérieure
    // Angle de stack de PI/2 (équateur) à 0 (pôle supérieur)
    for (unsigned int i = 0; i <= m_hemisphereStacks; ++i) {
        float stackAngle = PI / 2.0f - (float)i / m_hemisphereStacks * (PI / 2.0f);
        float xy = m_radius * cosf(stackAngle); // Rayon du cercle à cette hauteur de stack
        z = m_radius * sinf(stackAngle); // Hauteur z relative au centre de la demi-sphère

        // V pour la demi-sphère supérieure (de 0.75 à 1.0)
        // Quand i = 0 (équateur), sin(stackAngle)=1, v = 0.75f + 0.0f * 0.25f = 0.75f (si stackAngle = PI/2)
        // Ou plus simple: v = 0.75f + ((float)i / m_hemisphereStacks) * 0.25f; (si i=0 est pôle)
        // Si i=0 est équateur et i=m_hemisphereStacks est pôle :
        // v = 0.75f + ( (1.0f - (float)i / m_hemisphereStacks) * 0.25f );
        // Si i=0 est pôle et i=m_hemisphereStacks est équateur :
        v = 0.75f + ((float)i / m_hemisphereStacks) * 0.25f;

        for (unsigned int j = 0; j <= m_sectorCount; ++j) {
            float sectorAngle = (float)j * sectorStep;

            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle); // Dans le plan XY local de la sphère

            m_vertices.push_back(x);
            m_vertices.push_back(y); // Y sphérique devient Y du monde
            m_vertices.push_back(z + m_cylinderHeight / 2.0f); // Décalage pour être au-dessus du cylindre

            u = (float)j / m_sectorCount;
            m_vertices.push_back(u);
            m_vertices.push_back(v);
        }
    }

    // Indices pour les triangles de la demi-sphère supérieure
    std::vector<float> expanded_vertices;
    unsigned int baseIndex = 0; // Les sommets de la demi-sphère sup sont au début de m_vertices
    for (unsigned int i = 0; i < m_hemisphereStacks; ++i) {
        for (unsigned int j = 0; j < m_sectorCount; ++j) {
            unsigned int first = baseIndex + i * (m_sectorCount + 1) + j;
            unsigned int second = first + m_sectorCount + 1;

            // Triangle 1
            for(int k=0; k<5; ++k) expanded_vertices.push_back(m_vertices[first * 5 + k]);
            for(int k=0; k<5; ++k) expanded_vertices.push_back(m_vertices[second * 5 + k]);
            for(int k=0; k<5; ++k) expanded_vertices.push_back(m_vertices[(first + 1) * 5 + k]);

            // Triangle 2
            for(int k=0; k<5; ++k) expanded_vertices.push_back(m_vertices[(first + 1) * 5 + k]);
            for(int k=0; k<5; ++k) expanded_vertices.push_back(m_vertices[second * 5 + k]);
            for(int k=0; k<5; ++k) expanded_vertices.push_back(m_vertices[(second + 1) * 5 + k]);
        }
    }
    m_vertices = expanded_vertices; // Réaffecter les sommets expansés

    // 2. Partie cylindrique
    // Les sommets du cylindre sont stockés temporairement puis ajoutés
    std::vector<float> cylinder_temp_vertices;
    for (unsigned int i = 0; i <= m_cylinderStacks; ++i) {
        // z va de cylinderHeight/2.0f à -cylinderHeight/2.0f
        z = m_cylinderHeight / 2.0f - (float)i / m_cylinderStacks * m_cylinderHeight;

        // V pour le cylindre (de 0.75 à 0.25)
        v = 0.75f - ((float)i / m_cylinderStacks) * 0.5f;

        for (unsigned int j = 0; j <= m_sectorCount; ++j) {
            float sectorAngle = (float)j * sectorStep;
            x = m_radius * cosf(sectorAngle);
            y = m_radius * sinf(sectorAngle);

            cylinder_temp_vertices.push_back(x);
            cylinder_temp_vertices.push_back(y);
            cylinder_temp_vertices.push_back(z);

            u = (float)j / m_sectorCount;
            cylinder_temp_vertices.push_back(u);
            cylinder_temp_vertices.push_back(v);
        }
    }

    // Indices pour les triangles du cylindre
    baseIndex = 0; // Les sommets du cylindre sont maintenant dans cylinder_temp_vertices
    for (unsigned int i = 0; i < m_cylinderStacks; ++i) {
        for (unsigned int j = 0; j < m_sectorCount; ++j) {
            unsigned int first = baseIndex + i * (m_sectorCount + 1) + j;
            unsigned int second = first + m_sectorCount + 1;

            // Triangle 1
            for(int k=0; k<5; ++k) m_vertices.push_back(cylinder_temp_vertices[first * 5 + k]);
            for(int k=0; k<5; ++k) m_vertices.push_back(cylinder_temp_vertices[second * 5 + k]);
            for(int k=0; k<5; ++k) m_vertices.push_back(cylinder_temp_vertices[(first + 1) * 5 + k]);
            
            // Triangle 2
            for(int k=0; k<5; ++k) m_vertices.push_back(cylinder_temp_vertices[(first + 1) * 5 + k]);
            for(int k=0; k<5; ++k) m_vertices.push_back(cylinder_temp_vertices[second * 5 + k]);
            for(int k=0; k<5; ++k) m_vertices.push_back(cylinder_temp_vertices[(second + 1) * 5 + k]);
        }
    }

    // 3. Demi-sphère inférieure
    // Angle de stack de 0 (pôle inférieur) à -PI/2 (équateur)
    std::vector<float> bottom_hemi_temp_vertices;
    for (unsigned int i = 0; i <= m_hemisphereStacks; ++i) {
        // stackAngle de 0 (pôle) à -PI/2 (équateur)
        float stackAngle = -((float)i / m_hemisphereStacks * (PI / 2.0f));
        float xy = m_radius * cosf(stackAngle);
        z = m_radius * sinf(stackAngle); // z sera négatif

        // V pour la demi-sphère inférieure (de 0.0 à 0.25)
        // Si i=0 est pôle et i=m_hemisphereStacks est équateur
        v = ((float)i / m_hemisphereStacks) * 0.25f;

        for (unsigned int j = 0; j <= m_sectorCount; ++j) {
            float sectorAngle = (float)j * sectorStep;

            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);
            
            bottom_hemi_temp_vertices.push_back(x);
            bottom_hemi_temp_vertices.push_back(y);
            bottom_hemi_temp_vertices.push_back(z - m_cylinderHeight / 2.0f); // Décalage pour être en dessous

            u = (float)j / m_sectorCount;
            bottom_hemi_temp_vertices.push_back(u);
            bottom_hemi_temp_vertices.push_back(v);
        }
    }

    // Indices pour les triangles de la demi-sphère inférieure
    baseIndex = 0; // Les sommets sont dans bottom_hemi_temp_vertices
     for (unsigned int i = 0; i < m_hemisphereStacks; ++i) {
        for (unsigned int j = 0; j < m_sectorCount; ++j) {
            unsigned int first = baseIndex + i * (m_sectorCount + 1) + j;
            unsigned int second = first + m_sectorCount + 1;

            // Triangle 1
            for(int k=0; k<5; ++k) m_vertices.push_back(bottom_hemi_temp_vertices[first * 5 + k]);
            for(int k=0; k<5; ++k) m_vertices.push_back(bottom_hemi_temp_vertices[second * 5 + k]);
            for(int k=0; k<5; ++k) m_vertices.push_back(bottom_hemi_temp_vertices[(first + 1) * 5 + k]);
            
            // Triangle 2
            for(int k=0; k<5; ++k) m_vertices.push_back(bottom_hemi_temp_vertices[(first + 1) * 5 + k]);
            for(int k=0; k<5; ++k) m_vertices.push_back(bottom_hemi_temp_vertices[second * 5 + k]);
            for(int k=0; k<5; ++k) m_vertices.push_back(bottom_hemi_temp_vertices[(second + 1) * 5 + k]);
        }
    }
}

void CapsuleMesh::SetupMesh() {
    GenerateVertices(); // Génère les sommets dans m_vertices

    if (m_vertices.empty()) {
        // Gérer le cas où aucun sommet n'est généré (erreur ou paramètres non valides)
        std::cerr << "CapsuleMesh: No vertices generated." << std::endl;
        return;
    }

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);

    // Attribut de position des sommets
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Attribut de coordonnées de texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO
    glBindVertexArray(0); // Unbind VAO
}

void CapsuleMesh::Start() {
    SetupMesh();
}

void CapsuleMesh::Render(Shader& shader, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    if (m_VAO == 0 || m_vertices.empty()) return; // Ne rien faire si non initialisé

    shader.Use();
    if (m_texture) {
        m_texture->Bind(0);
    }

    shader.SetMat4("projection", projectionMatrix);
    shader.SetMat4("view", viewMatrix);

    // Assurez-vous que m_owner est défini et que GetTransform() est valide
    if (m_owner) {
        glm::mat4 modelMatrix = m_owner->GetTransform().GetModelMatrix();
        shader.SetMat4("model", modelMatrix);
   } else {
       // Gérer le cas où m_owner n'est pas défini, peut-être utiliser une matrice identité
       shader.SetMat4("model", glm::mat4(1.0f));
       // std::cerr << "CapsuleMesh::Render - m_owner is null!" << std::endl; // Optionnel: log
   }

   glBindVertexArray(m_VAO);

    // Le nombre de sommets à dessiner est m_vertices.size() / 5 (car chaque sommet a 5 floats: x,y,z,u,v)
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vertices.size() / 5));
    glBindVertexArray(0);

    if (m_texture) {
        m_texture->Unbind(0);
    }
}
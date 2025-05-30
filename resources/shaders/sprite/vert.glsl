#version 330 core

// Attributs de sommet
layout (location = 0) in vec3 aPos;        // Position du sommet (x, y, z)
layout (location = 1) in vec2 aTexCoord;   // Coordonnée de texture (u, v)

// Sortie vers le fragment shader
out vec2 TexCoord;

// Matrices uniformes
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // Transformation de la position du sommet
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    // Passage des coordonnées de texture au fragment shader
    TexCoord = aTexCoord;
}
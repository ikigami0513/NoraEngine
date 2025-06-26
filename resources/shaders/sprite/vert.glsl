#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 aColor; // Nouvelle entrée pour la couleur par sommet

out vec2 TexCoord;
out vec4 FragmentColor; // Passera la couleur au fragment shader

uniform mat4 projection;
uniform mat4 view;
// uniform mat4 model; // PLUS NÉCESSAIRE: la matrice modèle est appliquée sur le CPU

void main()
{
    gl_Position = projection * view * vec4(aPos, 1.0); // aPos est déjà en coordonnées mondiales
    TexCoord = aTexCoord;
    FragmentColor = aColor; // Passe la couleur du sommet au fragment shader
}
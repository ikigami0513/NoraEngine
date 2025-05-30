#version 330 core

// Entrée depuis le vertex shader (interpolée)
in vec2 TexCoord;

// Sortie (couleur finale du fragment)
out vec4 FragColor;

// Uniforme pour la texture du sprite
uniform sampler2D textureSampler; // Votre C++ fait m_texture->Bind(0), ce sampler utilisera l'unité de texture 0 par défaut.

void main() {
    // Échantillonnage de la texture aux coordonnées données
    FragColor = texture(textureSampler, TexCoord);
    
    // Optionnel: si vous voulez ignorer les pixels totalement transparents (meilleur pour certains types de blending)
    // if(FragColor.a < 0.1)
    //     discard;
}

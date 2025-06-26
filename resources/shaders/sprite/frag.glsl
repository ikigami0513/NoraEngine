#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 FragmentColor; // Reçoit la couleur du vertex shader

uniform sampler2D image;
uniform int useTexture; // 0 si pas de texture, 1 si texture

// uniform vec4 spriteColor; // PLUS NÉCESSAIRE

void main()
{
    if (useTexture == 1) {
        // La couleur du fragment est la couleur du sprite (FragmentColor) teintée par la texture
        FragColor = texture(image, TexCoord) * FragmentColor;
    } else {
        // Si pas de texture, utilise seulement la couleur du sprite
        FragColor = FragmentColor;
    }
}
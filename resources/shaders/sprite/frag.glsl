#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D image;
uniform vec4 spriteColor; // couleur pour teinter le sprite
uniform int useTexture;

void main() {
    if (useTexture == 1) {
        FragColor = texture(image, TexCoords) * spriteColor;
    }
    else {
        FragColor = spriteColor;
    }

    if (FragColor.a < 0.01)
        discard;
}

#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D image;
uniform vec4 spriteColor; // couleur pour teinter le sprite

void main() {
    vec4 texColor = texture(image, TexCoords);
    FragColor = texColor * spriteColor;
}

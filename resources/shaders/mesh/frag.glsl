#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in float FragDistance;

// texture samplers
uniform sampler2D texture1;

// Fog parameters
uniform vec3 fogColor;
uniform float minFogDist;
uniform float maxFogDist;
uniform bool hasFog;

void main() {
	vec4 baseColor = texture(texture1, TexCoord);

    if (hasFog) {
        float fogFactor = clamp((maxFogDist - FragDistance) / (maxFogDist - minFogDist), 0.0, 1.0);
        vec3 finalColor = mix(fogColor.rgb, baseColor.rgb, fogFactor);
        FragColor = vec4(finalColor, baseColor.a);
    } else {
        FragColor = baseColor;
    }
}

#version 400 core

in vec2 textureCoords;

out vec4 FragColour;

uniform sampler2D sampler;
uniform bool isDepthTexture;

const float near = 0.1f;
const float far = 1000.0f;

float linearDepth(float depth) {
    float z = depth * 2.0f - 1.0f;
    return (2.0f * near * far) / (far + near - z * (far - near));
}

void main(void) {
    vec4 colour = texture(sampler, textureCoords);
    if (isDepthTexture) {
        float depth = (linearDepth(colour.r) - linearDepth(gl_FragCoord.z)) / far;
        FragColour = vec4(vec3(depth), 1.0f);
    } else {
        FragColour = colour;
    }
}

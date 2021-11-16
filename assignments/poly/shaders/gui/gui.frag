#version 400 core

in vec2 textureCoords;

out vec4 FragColour;

uniform sampler2D sampler;

void main(void) {
    gl_FragColor = texture(sampler, textureCoords);
}

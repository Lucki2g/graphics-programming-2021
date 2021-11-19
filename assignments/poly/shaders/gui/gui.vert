#version 400 core

in vec2 in_position;

out vec2 textureCoords;

uniform mat4 transformationMatrix;

void main(void) {
    gl_Position = transformationMatrix * vec4(in_position, 0.0f, 1.0f);
    textureCoords = vec2((in_position.x + 1.0f) / 2.0f, 1.0f - (in_position.y + 1.0f) / 2.0f);
}

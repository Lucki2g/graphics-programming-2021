#version 400 core
in vec2 in_position;

out vec2 textureCoords;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main(void) {
    gl_Position = projectionMatrix * viewMatrix * transformationMatrix * vec4(in_position.x, 0.0, in_position.y, 1.0);
    textureCoords = vec2(in_position.x / 2.0f + 0.5f, in_position.y / 2.0f + 0.5f);
}

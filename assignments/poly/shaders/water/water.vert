#version 400 core
in vec2 in_position;

out vec4 clipSpace;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main(void) {
    clipSpace = projectionMatrix * viewMatrix * transformationMatrix * vec4(in_position.x, 0.0, in_position.y, 1.0);
    gl_Position = clipSpace;
}

#version 400 core
in vec2 in_position;

out vec4 clipSpace;
out vec3 toCamera;

uniform vec3 cameraPosition;
uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main(void) {
    vec4 worldPosition = transformationMatrix * vec4(in_position.x, 0.0, in_position.y, 1.0);
    clipSpace = projectionMatrix * viewMatrix * worldPosition;
    gl_Position = clipSpace;
    toCamera = cameraPosition - worldPosition.xyz;
}

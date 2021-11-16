#version 400 core

in vec3 in_position;
in vec3 in_normal;
in vec3 in_colour;

flat out vec3 out_colour;
out vec3 N;
out vec3 L;

uniform vec3 lightPosition;
uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main(void) {
    vec4 worldPosition = transformationMatrix * vec4(in_position, 1.0);
    gl_Position = projectionMatrix * viewMatrix * worldPosition;

    N = normalize(mat3(transpose(inverse(transformationMatrix))) * in_normal);
    L = normalize(lightPosition - worldPosition.xyz);

    out_colour = in_colour;
}

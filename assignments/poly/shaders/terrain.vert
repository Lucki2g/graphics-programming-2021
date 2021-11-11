#version 400 core

in vec3 in_position;
in vec3 in_normal;
in vec3 in_colour;

out vec3 surfaceNormal;
out vec3 toLightVector;
out vec3 out_colour;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform vec3 lightPosition;

void main(void) {

    vec4 worldPosition = transformationMatrix * vec4(in_position, 1.0);
    vec4 finalPos = projectionMatrix * viewMatrix * worldPosition;

    surfaceNormal = (transformationMatrix * vec4(in_normal, 1.0f)).xyz;
    toLightVector = lightPosition - worldPosition.xyz;

    out_colour = in_colour;
    gl_Position = finalPos;
}

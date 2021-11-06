#version 400 core

in vec3 position;

out vec3 outColor;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main(void) {

    vec4 finalPos = projectionMatrix * viewMatrix * transformationMatrix * vec4(position, 1.0);

    outColor = vec3(position.x+.5,1,position.y+.5);
    gl_Position = finalPos;
}

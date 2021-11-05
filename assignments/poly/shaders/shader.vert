#version 400 core

in vec3 position;

out vec3 outColor;

uniform mat4 transformationMatrix;

void main(void) {

    vec4 finalPos = transformationMatrix * vec4(position, 1.0);

    outColor = vec3(position.x+.5,1,position.y+.5);
    gl_Position = finalPos;
}

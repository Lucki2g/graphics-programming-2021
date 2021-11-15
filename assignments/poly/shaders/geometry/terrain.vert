#version 400 core

in vec3 in_position;
in vec3 in_colour;

out vec3 vertexColour;

void main(void) {
    gl_Position = vec4(in_position, 1.0f);
    vertexColour = in_colour.rgb;
}

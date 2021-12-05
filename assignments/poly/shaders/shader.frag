#version 400 core
out vec4 FragColor;

in vec3 final_colour;

void main(void) {
    FragColor = vec4(final_colour, 1.0);
}

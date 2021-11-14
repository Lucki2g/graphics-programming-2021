#version 400 core
out vec4 FragColor;
in vec3 out_colour;
void main(void) {
    FragColor = vec4(out_colour, 1.0);
}

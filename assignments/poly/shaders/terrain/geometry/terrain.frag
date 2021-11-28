#version 400 core
in vec3 finalColour;

out vec4 out_colour;

void main(void){
    out_colour = vec4(finalColour, 1.0);
}

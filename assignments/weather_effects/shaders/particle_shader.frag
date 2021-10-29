#version 330 core

in float blurSaturation;

out vec4 fragColor;

const vec3 color = vec3(1.0, 1.0, 1.0); // const color

void main()
{
   fragColor = vec4(color, blurSaturation);
}
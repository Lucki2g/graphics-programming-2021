#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 color;
out vec4 vtxColor;

uniform float boxSize;
uniform vec3 cameraPos;
uniform mat4 model;
uniform vec3 offsets;
uniform vec3 forwardOffset;

void main()
{
    vec3 position = mod(pos + offsets, boxSize);
    position += cameraPos + forwardOffset - (boxSize / 2);
    gl_Position = model * vec4(position, 1.f);
    gl_PointSize = 2.0f;
    vtxColor = color;
}
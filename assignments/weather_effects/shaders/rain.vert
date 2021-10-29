#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 color;
out vec4 vtxColor;
out float lenColorScale;

uniform float boxSize;
uniform vec3 cameraPos;
uniform vec3 cameraFront;
uniform mat4 model;
uniform mat4 modelPrev;
uniform vec3 offsets;
uniform vec3 velocity;
uniform vec3 forwardOffset;

void main()
{
    vec3 position = mod(pos + offsets, boxSize);
    position += cameraPos + forwardOffset - (boxSize / 2);

    float heightScale = .3f;
    vec3 positionPrev = position - velocity * heightScale;
   
    vec4 bottom  = model * vec4(position, 1.f);
    vec4 top     = model * vec4(positionPrev, 1.f);
    vec4 topPrev = modelPrev * vec4(positionPrev, 1.f);

    vec2 dir = (top.xy / top.w) - (bottom.xy / bottom.w);
    vec2 dirPrev = (topPrev.xy / topPrev.w) - (bottom.xy / bottom.w);
    float len = length(dir);
    float lenPrev = length(dirPrev);

    vec4 projPos;
    if (mod(gl_VertexID, 2) == 0) {
        projPos = topPrev;
    } else {
        projPos = bottom;
    };

    gl_Position = projPos;
    vtxColor = color;
    lenColorScale = clamp((len / lenPrev), 0.f, 1.f);
}
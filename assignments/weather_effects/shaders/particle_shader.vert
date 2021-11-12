#version 330 core
layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 prevModel;
uniform vec3 offset;
uniform vec3 camForward;
uniform vec3 camPosition;
uniform float boxSize;
uniform int moving;

out float blurSaturation;

void main()
{
   // check if vertx is top or bottom vertex
   int topVertex = int(gl_VertexID % 2 == 0);

   vec3 position = mod(pos + offset, vec3(boxSize));
   position += camPosition + camForward - boxSize / 2;
   vec3 prevPosition = mod(pos + offset + vec3(0, 0.02f, 0), vec3(boxSize));
   prevPosition += camPosition + camForward - boxSize / 2;

   // set pointsize based of distance to camera
   // gl_PointSize = distance(camPosition, position) * 2; - used for points

   // set position
   vec4 topChoice = mix(top, topPrev, moving);
   gl_Position = mix(bottom, topChoice, topVertex);
}
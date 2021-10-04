#version 330 core
layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform vec3 offset;
uniform vec3 camForward;
uniform vec3 camPosition;
uniform float boxSize;

void main()
{
   vec3 offsets = offset;
   offsets -= camPosition + camForward + boxSize / 2;
   // place offset in range [-boxSize, boxSize]
   offsets = mod(offsets, boxSize);

   vec3 position = mod(pos + offset, vec3(boxSize));
   position += camPosition + camForward - boxSize / 2;

   // set position
   gl_Position = model * vec4(position, 1.0);

   // set pointsize based of distance to camera
   gl_PointSize = distance(camPosition, position) * 2;
}
#version 330 core
out vec4 FragColor;
in  vec4 vtxColor;
void main()
{
   vec4 color = vec4(vtxColor.xyz, 1.f);
   FragColor = color;
}
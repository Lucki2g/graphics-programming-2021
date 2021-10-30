#version 330 core
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textCoord; // here for completness, but we are not using it just yet

uniform mat4 model; // represents model coordinates in the world coord space
uniform mat4 invTransposeModel; // inverse of the transpose of  model (used to multiply vectors while preserving angles)
uniform mat4 view;  // represents the world coordinates in the camera coord space
uniform mat4 projection; // camera projection matrix
uniform vec3 camPosition; // so we can compute the view vector (could be extracted from view matrix, but let's make our life easier :) )

// send shaded color to the fragment shader
out vec4 shadedColor;

// light uniform variables
uniform vec3 ambientColor;
uniform vec3 reflectionColor;
uniform float ambientReflectance;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float diffuseReflectance;

uniform float specularReflectance;
uniform float specularExponent;

// material properties

// attenuation uniforms
uniform vec3 lightPos2;
uniform vec3 lightColor2;


void main() {
   // vertex in world space (for light computation)
   vec4 P = model * vec4(vertex, 1.0);
   // normal in world space (for light computation)
   vec3 N = normalize((invTransposeModel * vec4(normal, 0.0)).xyz);

   // final vertex transform (for opengl rendering, not for lighting)
   gl_Position = projection * view * P;
   vec3 P3 = vec3(P);

   // TODO exercises 8.1, 8.2 and 8.3 - Gouraud shading (i.e. Phong reflection model computed in the vertex shader)

   // TODO 8.1 ambient
   vec3 ambient = vec3(ambientColor * reflectionColor * ambientReflectance);

   // TODO 8.2 diffuse
   vec3 L = normalize(lightPos - P3);
   vec3 diffuse = vec3(lightColor * diffuseReflectance * dot(N, L));

   // TODO 8.3 specular - something seems of here .------.?
   vec3 V = normalize(camPosition - P3);
   vec3 R = 2 * N * dot(N, L) - L;
   // vec3 R = reflect(-L, N);
   vec3 specularPhong = lightColor * specularReflectance * pow(max(dot(R, V), 0.0), specularExponent);

   // Blinn specular
   vec3 H = (L + V) / length(L + V);
   vec3 specularBlinn = lightColor * specularReflectance * pow(dot(N, H), specularExponent);

   // TODO exercise 8.6 - attenuation - light 2
   vec3 L2 = normalize(lightPos2 - P3);
   vec3 diffuse2 = vec3(lightColor2 * diffuseReflectance * dot(N, L2));

   vec3 R2 = 2 * N * dot(N, L2) - L2;
   vec3 specularPhong2 = lightColor * specularReflectance * pow(max(dot(R2, V), 0.0), specularExponent);


   // TODO set the output color to the shaded color that you have computed
   shadedColor = vec4((ambient + (diffuse + specularPhong) + (diffuse2 + specularPhong2)) * vec3(.8), 1.0);

}
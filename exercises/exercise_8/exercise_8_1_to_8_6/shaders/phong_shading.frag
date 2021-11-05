#version 330 core

uniform vec3 camPosition; // so we can compute the view vector
out vec4 FragColor; // the output color of this fragment

// TODO exercise 8.4 setup the 'uniform' variables needed for lighting
// light uniforms
in vec3 N;
in vec4 P;

uniform vec3 ambientColor;
uniform vec3 reflectionColor;
uniform float ambientReflectance;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float diffuseReflectance;

uniform float specularReflectance;
uniform float specularExponent;

// material uniforms

// attenuation uniforms

// TODO exercise 8.4 add the 'in' variables to receive the interpolated Position and Normal from the vertex shader


void main()
{
   // TODO exercise 8.4 - phong shading (i.e. Phong reflection model computed in the fragment shader)
   // ambient component
   vec3 ambient = vec3(ambientColor * reflectionColor * ambientReflectance);

   // diffuse component for light 1
   vec3 L = normalize(lightPos - vec3(P));
   vec3 diffuse = vec3(lightColor * diffuseReflectance * dot(N, L));

   // specular component for light 1
   vec3 V = normalize(camPosition - vec3(P));
   vec3 R = 2 * N * dot(N, L) - L;
   // vec3 R = reflect(-L, N);
   vec3 specularPhong = lightColor * specularReflectance * pow(max(dot(R, V), 0.0), specularExponent);

   // Blinn specular
   vec3 H = (L + V) / length(L + V);
   vec3 specularBlinn = lightColor * specularReflectance * pow(dot(N, H), specularExponent);

   // TODO exercise 8.5 - multiple lights, compute diffuse and specular of light 2


   // TODO exercuse 8.6 - attenuation - light 1


   // TODO exercuse 8.6 - attenuation - light 2


   // TODO compute the final shaded color (e.g. add contribution of the (attenuated) lights 1 and 2)


   // TODO set the output color to the shaded color that you have computed
   FragColor = vec4(ambient + diffuse + specularPhong, 1.0);
}
// you might have noticed that the shading contribution of multiple lights can fit a for loop nicely
// we will be doing that later on
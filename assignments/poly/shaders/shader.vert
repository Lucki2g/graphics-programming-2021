#version 400 core

in vec3 in_position;
in vec3 in_normal;

out vec3 final_colour;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform vec3 lightColour;
uniform vec3 lightDirection;
uniform vec3 ambientLightColour;
uniform float ambientReflectance;
uniform float diffuseReflectance;

vec3 getLighting() {

    vec3 N = normalize(mat3(transpose(inverse(transformationMatrix))) * in_normal);

    // ambient
    vec3 ambient = lightColour * ambientReflectance;

    // diffuse
    float diff = max(dot(-lightDirection, N), 0.0);
    vec3 diffuse = diff * diffuseReflectance * lightColour;

    return (ambient + diffuse);
}


void main(void) {
    vec4 worldPosition = transformationMatrix * vec4(in_position, 1.0);
    vec4 finalPos = projectionMatrix * viewMatrix * worldPosition;
    gl_Position = finalPos;

    final_colour = vec3(1.0f) * getLighting();
}

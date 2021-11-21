#version 400 core

in vec3 in_position;
in vec3 in_normal;
in vec3 in_colour;

flat out vec3 out_colour;

uniform vec3 lightPosition;
uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform vec3 lightColour;
uniform vec3 ambientLightColour;
uniform float ambientReflectance;
uniform float diffuseReflectance;

const vec3 lightDirection = vec3(0.3f, -1f, 0.5f);

vec3 getLight(vec3 L, vec3 N) {
    // ambient
    vec3 ambient = lightColour * ambientReflectance;

    // diffuse
    float diff = max(dot(-L, N), 0.0);
    vec3 diffuse = diff * diffuseReflectance * lightColour;

    return (ambient + diffuse) * in_colour;
}

void main(void) {
    vec4 worldPosition = transformationMatrix * vec4(in_position, 1.0);
    gl_Position = projectionMatrix * viewMatrix * worldPosition;

    vec3 N = in_normal * 2.0f - 1.0f;

    out_colour = getLight(lightDirection, N);
}

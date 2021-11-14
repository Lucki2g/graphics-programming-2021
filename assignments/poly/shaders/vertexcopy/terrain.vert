#version 400 core

in vec3 in_position;
in vec3 in_normal;
in vec3 in_colour;

out vec3 out_colour;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform vec3 lightColour;
uniform vec3 lightPosition;
uniform vec3 ambientLightColour;
uniform float ambientReflectance;
uniform float diffuseReflectance;

vec3 getLight(vec3 lightDir) {
    vec3 normal = in_normal.xyz * 2.0 - 1.0;
    float brightness = max(dot(lightDir, (transformationMatrix * vec4(normal, 1.0)).xyz), 0.0);
    return lightColour + (brightness * lightColour * diffuseReflectance);
}

void main(void) {

    vec4 worldPosition = transformationMatrix * vec4(in_position, 1.0);
    vec4 finalPos = projectionMatrix * viewMatrix * worldPosition;

    vec3 toLightVector = lightPosition - worldPosition.xyz;

    out_colour = in_colour * getLight(toLightVector);
    gl_Position = finalPos;
}

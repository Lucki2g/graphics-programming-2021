#version 400 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_colour;

out vec3 out_colour;

uniform vec3 lightPosition;
uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform vec3 lightColour;
uniform vec3 ambientLightColour;
uniform float ambientReflectance;
uniform float diffuseReflectance;

uniform vec4 waterClippingPlane; // used for plane equation (normal, height)

const vec3 lightDirection = vec3(0.3f, -1f, 0.5f);

vec3 getLight() {

    vec3 N = in_normal.xyz;

    // ambient
    vec3 ambient = lightColour * ambientReflectance;

    // diffuse
    float diff = max(dot(lightDirection, N), 0.0);
    vec3 diffuse = diff * diffuseReflectance * lightColour;

    return (ambient + diffuse);
}

void main(void) {
    gl_Position = projectionMatrix * viewMatrix * vec4(in_position, 1.0f);

    gl_ClipDistance[0] = dot(vec4(in_position, 1.0f), waterClippingPlane); // test for clipping

    vec3 lighting = getLight();
    out_colour = lighting * in_colour;
}

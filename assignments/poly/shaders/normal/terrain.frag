#version 400 core
out vec4 FragColor;

in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 out_colour;

uniform vec3 lightColour;

uniform vec3 ambientLightColour;
uniform float ambientReflectance;

uniform float diffuseReflectance;

vec3 lighting() {
    // ambient
    vec3 ambient = ambientLightColour * ambientReflectance * out_colour;

    // diffuse light 1
    vec3 L = normalize(toLightVector);
    vec3 N = normalize(surfaceNormal);
    float brightness = max(dot(N, L), 0.0f);
    vec3 diffuse = lightColour * diffuseReflectance * brightness * out_colour;

    return ambient + diffuse;
    // return (lightColour * 0.3f) + (brightness * lightColour * 0.8f);
}

void main(void) {
    vec3 light = lighting();
    FragColor = vec4(light, 1.0);
}

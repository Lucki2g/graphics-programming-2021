#version 400 core
out vec4 FragColor;

in vec3 out_colour;
in vec3 N;
in vec3 L;

uniform vec3 lightColour;
uniform vec3 ambientLightColour;
uniform float ambientReflectance;
uniform float diffuseReflectance;

vec3 getLight() {
    // ambient
    vec3 ambient = ambientLightColour * ambientReflectance;

    // diffuse
    float diff = max(dot(L, N), 0.0);
    vec3 diffuse = diff * diffuseReflectance * lightColour;

    return (ambient + diffuse) * out_colour;
}

void main(void) {
    FragColor = vec4(getLight(), 1.0);
}

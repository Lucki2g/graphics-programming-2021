#version 400 core
out vec4 FragColor;

in vec3 surfaceNormal;
in vec3 toLightVector;

uniform vec3 lightColour;

void main(void) {

    vec3 N = normalize(surfaceNormal);
    vec3 L = normalize(toLightVector);

    float brightness = max(dot(N, L), 0.0f);

    vec3 color = vec3(0, 0, 0);
    vec3 diffuse = brightness * lightColour;

    FragColor = vec4(color, 1.0);
}

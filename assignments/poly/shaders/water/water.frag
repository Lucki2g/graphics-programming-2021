#version 400 core

in vec4 clipSpace;
in vec3 toCamera;

out vec4 FragColor;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

void main(void) {

    vec2 normalisedDeviceSpace = clipSpace.xy/clipSpace.w; // perspective division (results in coords in [-1:1])
    vec2 sampleCoordinates = normalisedDeviceSpace / 2.0f + 0.5f;

    vec2 refractTexCoordinates = sampleCoordinates.xy;
    vec2 reflectTexCoordinates = vec2(sampleCoordinates.x, -sampleCoordinates.y);

    vec4 reflectColour = texture(reflectionTexture, reflectTexCoordinates);
    vec4 refractColour = texture(refractionTexture, refractTexCoordinates);

    // blending teqnuiqe
    vec3 viewVector = normalize(toCamera);
    float fresnel = dot(viewVector, vec3(0.0f, 1.0f, 0.0f));
    fresnel = pow(fresnel, 5.0f); // increase reflectiveness

    FragColor = mix(reflectColour, refractColour, fresnel);
}
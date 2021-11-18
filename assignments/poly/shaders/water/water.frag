#version 400 core

in vec4 clipSpace;

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

    FragColor = mix(reflectColour, refractColour, 0.5f);
}
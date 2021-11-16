#version 400 core

in vec2 textureCoords;

out vec4 FragColor;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

void main(void) {

    vec4 reflectColour = texture(reflectionTexture, textureCoords);
    vec4 refractColour = texture(refractionTexture, textureCoords);

    FragColor = reflectColour;//mix(reflectColour, refractColour, 0.5f);
}
#version 400 core

in vec4 pass_clipSpace;
in vec4 pass_clipSpaceGrid;
in vec3 pass_toCamera;
in vec3 pass_normal;

out vec4 FragColor;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D depthTexture;
uniform vec2 nearFarPlane;

const vec3 waterColour = vec3(0.604, 0.867, 0.851);
const float fresnelReflectiveness = 0.65f;
const float softness = 1.0f;
const float minBlueness = 0.4f;
const float maxBlueness = 0.75f;
const float murkyDepth = 15f;

vec3 applyMurk(vec3 refractColour, float waterDepth) {
    float murkFactor = smoothstep(0.0f, murkyDepth, waterDepth);
    float murkiness = minBlueness + murkFactor * (maxBlueness - minBlueness);
    return mix(refractColour, waterColour, murkiness);
}

float linearDepth(float zDepth) {
    float near = nearFarPlane.x;
    float far = nearFarPlane.y;
    return 2.0f * near * far / (far + near - (2.0f * zDepth - 1.0f) * (far - near)); // the function is reverse exponentiel (needs to be made linear)
}

float getDepth(vec2 texCoord) {
    float depth = texture(depthTexture, texCoord).r;
    float floorDistance = linearDepth(depth); // distance to floor from camera
    float waterDistance = linearDepth(gl_FragCoord.z); // distance to water from camera
    return floorDistance - waterDistance; // distance from water to floor
}

vec2 getClipSpaceTexCoordinates(vec4 space) {
    vec2 normalisedDeviceSpace = space.xy / space.w; // perspective division (results in coords in [-1:1])
    vec2 sampleCoordinates = normalisedDeviceSpace / 2.0f + 0.5f; // convert to [0:1]
    return clamp(sampleCoordinates, 0.002f, 0.998f);
}

void main(void) {

    vec2 texCoordinatesGrid = getClipSpaceTexCoordinates(pass_clipSpaceGrid);
    vec2 texCoordinates = getClipSpaceTexCoordinates(pass_clipSpace);

    vec2 refractTexCoordinates = texCoordinatesGrid.xy;
    vec2 reflectTexCoordinates = vec2(texCoordinatesGrid.x, -texCoordinatesGrid.y);
    float waterDepth = getDepth(texCoordinates);

    vec3 reflectColour = texture(reflectionTexture, reflectTexCoordinates).rgb;
    vec3 refractColour = texture(refractionTexture, refractTexCoordinates).rgb;

    // depth effects
    refractColour = applyMurk(refractColour, waterDepth); // apply water colour based on depth in water
    reflectColour = mix(reflectColour, waterColour, minBlueness); // apply water colour to reflections

    // blending technuiqe (Fresnel)
    vec3 viewVector = normalize(pass_toCamera);
    float fresnel = dot(viewVector, normalize(pass_normal));
    fresnel = pow(fresnel, fresnelReflectiveness); // increase reflectiveness
    fresnel = clamp(fresnel, 0.0, 1.0);

    // soft edges
    float alpha = clamp(waterDepth / softness, 0.0f, 1.0f);

    vec4 final_colour = vec4((mix(reflectColour, refractColour, fresnel)), alpha); // actual
    //vec4 final_colour = vec4(out_normal, 1.0f); // normals
    //vec4 final_colour = vec4(mix(vec3(0.0f), vec3(1.0f), waterDepth), 1.0f); // depth
    FragColor = final_colour;
}
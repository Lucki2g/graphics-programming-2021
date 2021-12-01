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
const float fresnelReflectiveness = 0.5f;
const float softness = 1.0f;
const float minBlueness = 0.4f;
const float maxBlueness = 0.75f;
const float murkyDepth = 15f;

vec3 applyMurkiness(vec3 refractColour, float waterDepth){
    float murkyFactor = smoothstep(0, murkyDepth, waterDepth);
    float murkiness = minBlueness + murkyFactor * (maxBlueness - minBlueness);
    return mix(refractColour, waterColour, murkiness);
}

float toLinearDepth(float zDepth){
    float near = nearFarPlane.x;
    float far = nearFarPlane.y;
    return 2.0 * near * far / (far + near - (2.0 * zDepth - 1.0) * (far - near));
}

float calculateWaterDepth(vec2 texCoords){
    float depth = texture(depthTexture, texCoords).r;
    float floorDistance = toLinearDepth(depth);
    depth = gl_FragCoord.z;
    float waterDistance = toLinearDepth(depth);
    return floorDistance - waterDistance;
}

vec2 getClipSpaceTexCoordinates(vec4 space) {
    vec2 normalisedDeviceSpace = space.xy / space.w; // perspective division (results in coords in [-1:1])
    vec2 sampleCoordinates = normalisedDeviceSpace / 2.0f + 0.5f; // convert to [0:1]
    return clamp(sampleCoordinates, 0.002f, 0.998f);
}

void main(void) {

    vec2 texCoordinates = getClipSpaceTexCoordinates(pass_clipSpace);

    vec2 refractTexCoordinates = texCoordinates.xy;
    vec2 reflectTexCoordinates = vec2(texCoordinates.x, 1.0 - texCoordinates.y);
    float waterDepth = calculateWaterDepth(texCoordinates);

    vec3 reflectColour = texture(reflectionTexture, reflectTexCoordinates).rgb;
    vec3 refractColour = texture(refractionTexture, refractTexCoordinates).rgb;
    refractColour = applyMurkiness(refractColour, waterDepth);
    reflectColour = mix(reflectColour, waterColour, minBlueness);

    // blending technuiqe (Fresnel)
    vec3 viewVector = normalize(pass_toCamera);
    float fresnel = dot(viewVector, normalize(pass_normal));
    fresnel = pow(fresnel, fresnelReflectiveness); // increase reflectiveness
    fresnel = clamp(fresnel, 0.0, 1.0);

    vec4 final_colour = vec4((mix(reflectColour, refractColour, fresnel)), 1.0f); // actual
    FragColor = final_colour;
    FragColor.a = clamp(waterDepth / softness, 0.0, 1.0);
}
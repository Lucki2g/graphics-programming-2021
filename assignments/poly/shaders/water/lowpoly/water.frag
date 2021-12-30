#version 400 core

in vec4 pass_clipSpace;
in vec4 pass_clipSpaceGrid;
in vec3 pass_toCamera;
in vec3 pass_normal;
in vec3 pass_specular;
in vec3 pass_ambientdiffuse;

out vec4 FragColor;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D depthTexture;
uniform vec2 nearFarPlane;
uniform vec3 waterColour;
uniform float fresnelReflectiveness;

const float softness = 1.0f;
const float minBlueness = 0.4f;
const float maxBlueness = 0.75f;
const float murkyDepth = 12.0f;

// depth effect: The deeper the bluer
vec3 applyMurkiness(vec3 refractColour, float waterDepth){
    float murkyFactor = smoothstep(0, murkyDepth, waterDepth);
    float murkiness = minBlueness + murkyFactor * (maxBlueness - minBlueness);
    return mix(refractColour, waterColour, murkiness);
}

// Exponential to linear function
float toLinearDepth(float zDepth){
    float near = nearFarPlane.x;
    float far = nearFarPlane.y;
    return (2.0 * near * far) / (far + near - (2.0 * zDepth - 1.0) * (far - near));
}

// Get the depth from water to submerged terrain
float calculateWaterDepth(vec2 texCoords){
    float depth = texture(depthTexture, texCoords).r;
    float floorDistance = toLinearDepth(depth);
    depth = gl_FragCoord.z;
    float waterDistance = toLinearDepth(depth);
    return floorDistance - waterDistance;
}

// get coordinate in NDC
vec2 getClipSpaceTexCoordinates(vec4 space) {
    vec2 normalisedDeviceSpace = space.xy / space.w; // perspective division (results in coords in [-1:1])
    vec2 sampleCoordinates = normalisedDeviceSpace / 2.0f + 0.5f; // convert to [0:1]
    return clamp(sampleCoordinates, 0.002f, 0.998f); // fixed bleed bottom of screen
}

void main(void) {

    vec2 texCoordinates = getClipSpaceTexCoordinates(pass_clipSpace);
    vec2 texCoordinatesGrid = getClipSpaceTexCoordinates(pass_clipSpaceGrid);

    vec2 refractTexCoordinates = texCoordinatesGrid.xy;
    vec2 reflectTexCoordinates = vec2(texCoordinatesGrid.x, 1.0 - texCoordinatesGrid.y);
    float waterDepth = calculateWaterDepth(texCoordinates);

    vec3 reflectColour = texture(reflectionTexture, reflectTexCoordinates).rgb;
    vec3 refractColour = texture(refractionTexture, refractTexCoordinates).rgb;

    // depth effects
    refractColour = applyMurkiness(refractColour, waterDepth);
    reflectColour = mix(reflectColour, waterColour, minBlueness);

    // blending technuiqe (Fresnel)
    vec3 viewVector = normalize(pass_toCamera);
    float fresnel = dot(viewVector, normalize(pass_normal));
    fresnel = pow(fresnel, fresnelReflectiveness); // increase reflectiveness
    fresnel = clamp(fresnel, 0.0, 1.0);

    // colouring and lighting
    vec4 final_colour = vec4((mix(reflectColour, refractColour, fresnel) * (pass_ambientdiffuse + pass_specular)), 1.0f); // actual
    FragColor = final_colour;
    FragColor.a = clamp(waterDepth / softness, 0.0, 1.0);
}
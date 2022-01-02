#version 400 core
in vec2 in_position;
in vec4 in_indicators; // indicates the two other vertices of the triangle

out vec4 pass_clipSpace;
out vec4 pass_clipSpaceGrid;
out vec3 pass_toCamera;
out vec3 pass_normal;
out vec3 pass_specular;
out vec3 pass_ambientdiffuse;

// matrices and camera
uniform vec3 cameraPosition;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform float height;
uniform float waveTime;

// lighting
uniform vec3 lightColour;
uniform vec3 lightDirection;
uniform vec3 ambientLightColour;
uniform float ambientReflectance;
uniform float diffuseReflectance;
uniform float specularReflectance;
uniform float specularFactor;

// distortion
uniform int distortion;
const int RANDOM = 0;
const int SIN = 1;
uniform float waveLength;
uniform float waveAmplitude;

const float PI = 3.1415926535897932384626433832795;

// specular lighting
vec3 calcSpecularLighting(vec3 toCamVector, vec3 toLightVector, vec3 normal){
    vec3 reflectDirection = reflect(-toLightVector, normal);
    float specular = pow(max(dot(toCamVector, reflectDirection), 0.0f), specularFactor);
    return lightColour * specularReflectance * specular;
}

// ambient diffuse lighting
vec3 calcAmbientDiffuseLighting(vec3 N) {
    // ambient
    vec3 ambient = lightColour * ambientReflectance;

    // diffuse
    float diff = max(dot(-lightDirection, N), 0.0);
    vec3 diffuse = diff * diffuseReflectance * lightColour;

    return (ambient + diffuse);
}

vec3 calcNormal(vec3 v, vec3 v1, vec3 v2) {
    vec3 tangent1 = v1 - v;
    vec3 tangent2 = v2 - v;
    vec3 normal = cross(tangent1, tangent2);
    return normalize(normal);
}

// Distortion
// Equilinox random
float generateOffsetRandom(float x, float z, float val1, float val2){
    float radiansX = ((mod(x + z * x * val1, waveLength)/waveLength) + waveTime * mod(x * 0.8 + z, 1.5)) * 2.0 * PI;
    float radiansZ = ((mod(val2 * (z * x + x * z), waveLength)/waveLength) + waveTime * 2.0 * mod(x , 2.0) ) * 2.0 * PI;
    return waveAmplitude * 0.5 * (sin(radiansZ) + cos(radiansX));
}
// sinwave function
float generateOffsetSinus(float x, float z) {
    float radiansX = (x / waveLength + waveTime) * 2.0 * PI;
    float radiansZ = (z / waveLength + waveTime) * 2.0 * PI;
    return waveAmplitude * 0.5 * (sin(radiansZ) + cos(radiansX));
}

vec3 applyDistortion(vec3 vertex){
    float xDistortion, yDistortion, zDistortion;
    switch (distortion) {
        case RANDOM:
            xDistortion = generateOffsetRandom(vertex.x, vertex.z, 0.2, 0.1);
            yDistortion = generateOffsetRandom(vertex.x, vertex.z, 0.1, 0.3);
            zDistortion = generateOffsetRandom(vertex.x, vertex.z, 0.15, 0.2);
            break;
        case SIN:
            xDistortion = generateOffsetSinus(vertex.x, vertex.z);
            yDistortion = generateOffsetSinus(vertex.x, vertex.z);
            zDistortion = generateOffsetSinus(vertex.x, vertex.z);
            break;
    }
    return vertex + vec3(xDistortion, yDistortion, zDistortion);
}

void main(void) {

    vec3 vertex = vec3(in_position.x, height, in_position.y);
    vec3 v1 = vertex + vec3(in_indicators.x, 0.0f, in_indicators.y);
    vec3 v2 = vertex + vec3(in_indicators.z, 0.0f, in_indicators.w);
    pass_clipSpaceGrid = projectionMatrix * viewMatrix * vec4(vertex, 1.0f); // real grid position

    // distortion
    vertex = applyDistortion(vertex);
    v1 = applyDistortion(v1);
    v2 = applyDistortion(v2);

    pass_normal = -calcNormal(vertex, v1, v2);

    pass_clipSpace = projectionMatrix * viewMatrix * vec4(vertex, 1.0f);
    gl_Position = pass_clipSpace;

    pass_toCamera = normalize(cameraPosition - vertex);
    pass_specular = calcSpecularLighting(pass_toCamera, -lightDirection, pass_normal);
    pass_ambientdiffuse = calcAmbientDiffuseLighting(pass_normal);
}

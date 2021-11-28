#version 400 core
in vec2 in_position;
in vec4 in_indicators; // indicates the two other vertices of the triangle

out vec4 pass_clipSpace;
out vec4 pass_clipSpaceGrid;
out vec3 pass_toCamera;
out vec3 pass_normal;

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

const float PI = 3.1415926535897932384626433832795f;
const float waveLength = 10.0f;
const float waveAmplitude = 1.0f;

float createOffset(float x, float z, float val1, float val2) {
    float radiansX = (x / waveLength + waveTime) * 2.0 * PI;
    float radiansZ = (z / waveLength + waveTime) * 2.0 * PI;
    return waveAmplitude * 0.5f * (sin(radiansZ) + cos(radiansX));
}

vec3 distort(vec3 vertex) {
    float distX = createOffset(vertex.x, vertex.z, 0.2f, 0.1f);
    float distY = createOffset(vertex.x, vertex.z, 0.1f, 0.3f);
    float distZ = createOffset(vertex.x, vertex.z, 0.15f, 0.2f);
    return vertex + vec3(distX, distY, distZ);
}

vec3 calcNormal(vec3 v, vec3 v1, vec3 v2) {
    vec3 tangent1 = v1 - v;
    vec3 tangent2 = v2 - v;
    vec3 normal = cross(tangent1, tangent2);
    return normalize(normal);
}

void main(void) {

    vec3 vertex = vec3(in_position.x, height, in_position.y);
    vec3 offset1 = vec3(in_indicators.x, 0.0f, in_indicators.y);
    vec3 offset2 = vec3(in_indicators.z, 0.0f, in_indicators.w);
    vec3 v1 = vertex + offset1;
    vec3 v2 = vertex + offset2;

    pass_clipSpaceGrid = projectionMatrix * viewMatrix * vec4(vertex, 1.0f);

    vertex = distort(vertex);
    v1 = distort(v1);
    v2 = distort(v2);

    pass_normal = calcNormal(vertex, v1, v2);

    pass_clipSpace = projectionMatrix * viewMatrix * vec4(vertex, 1.0f);
    gl_Position = pass_clipSpace;

    pass_toCamera = normalize(cameraPosition - vertex);
}

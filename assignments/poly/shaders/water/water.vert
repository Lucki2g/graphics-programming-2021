#version 400 core
in vec2 in_position;
in vec4 in_indicators; // indicates the two other vertices of the triangle

out vec4 clipSpace;
out vec4 clipSpaceGrid;
out vec3 toCamera;
out vec3 out_normal;

uniform vec3 cameraPosition;
uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform float height;
uniform float waveTime;

const float PI = 3.1415926535897932384626433832795f;
const float waveLength = 10.0f;
const float waveAmplitude = 1.0f;

float createOffset(float x, float z) {
    float radiansX = (x / waveLength + waveTime) * 2.0f * PI;
    float radiansZ = (z / waveLength + waveTime) * 2.0f * PI;
    return waveAmplitude * 0.5f * (sin(radiansZ) + cos(radiansX));
}

vec3 distort(vec3 vertex) {
    float distX = createOffset(vertex.x, vertex.z);
    float distY = createOffset(vertex.x, vertex.z);
    float distZ = createOffset(vertex.x, vertex.z);
    return vertex + vec3(distX, distY, distZ);
}

vec3 calcNormal(vec3 v, vec3 v1, vec3 v2) {
    vec3 tangent1 = v1 - v;
    vec3 tangent2 = v2 - v;
    vec3 normal = cross(tangent1, tangent2);
    return normalize(normal);
}

void main(void) {

    vec3 vertex = (transformationMatrix * vec4(vec3(in_position.x, height, in_position.y), 1.0)).xyz;
    vec3 v1 = vertex + vec3(in_indicators.x, 0.0f, in_indicators.y);
    vec3 v2 = vertex + vec3(in_indicators.z, 0.0f, in_indicators.w);

    clipSpaceGrid = projectionMatrix * viewMatrix * vec4(vertex, 1.0f);

    vertex = distort(vertex);
    v1 = distort(v1);
    v2 = distort(v2);

    out_normal = calcNormal(vertex, v1, v2);

    clipSpace = projectionMatrix * viewMatrix * vec4(vertex, 1.0f);

    gl_Position = clipSpace;
    toCamera = normalize(cameraPosition - vertex);
}

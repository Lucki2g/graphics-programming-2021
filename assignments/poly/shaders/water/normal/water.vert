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

vec3 calcNormal(vec3 v, vec3 v1, vec3 v2) {
    vec3 tangent1 = v1 - v;
    vec3 tangent2 = v2 - v;
    vec3 normal = cross(tangent1, tangent2);
    return normalize(normal);
}

void main(void) {

    vec3 vertex = vec3(in_position.x, height, in_position.y);

    pass_normal = vec3(0.0f, 1.0f, 0.0f);

    pass_clipSpace = projectionMatrix * viewMatrix * vec4(vertex, 1.0f);
    gl_Position = pass_clipSpace;

    pass_toCamera = normalize(cameraPosition - vertex);
}

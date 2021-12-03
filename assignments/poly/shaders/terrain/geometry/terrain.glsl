#version 400 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 vertexColour[];
out vec3 finalColour;

uniform vec3 lightColour;
uniform vec3 lightDirection;
uniform vec3 ambientLightColour;
uniform float ambientReflectance;
uniform float diffuseReflectance;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;


vec3 getLight(vec3 N) {
    // ambient
    vec3 ambient = lightColour * ambientReflectance;

    // diffuse
    float diff = max(dot(lightDirection, N), 0.0);
    vec3 diffuse = diff * diffuseReflectance * lightColour;

    return (ambient + diffuse);
}

vec3 calcNormal() {
    vec3 tangent1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 tangent2 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 normal = cross(tangent1, tangent2);
    return normalize(normal);
}

void main(void) {
    vec3 normal = calcNormal();

    for (int i = 0; i < 3; i++) {
        vec3 light = getLight(normal);
        gl_Position = projectionMatrix * viewMatrix * transformationMatrix * gl_in[i].gl_Position;
        finalColour = vertexColour[0] * light;
        EmitVertex();
    }

    EndPrimitive();
}

#version 400 core

in vec3 in_position;
in vec3 in_normal;
in vec3 in_colour;

out vec3 out_colour;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform vec3 lightDirection;
uniform vec3 lightColour;
uniform vec3 ambientLightColour;
uniform float ambientReflectance;
uniform float diffuseReflectance;

uniform vec4 waterClippingPlane; // used for plane equation (normal, height)

vec3 getLight() {

    vec3 N = normalize(mat3(transpose(inverse(transformationMatrix))) * in_normal);

    // ambient
    vec3 ambient = lightColour * ambientReflectance;

    // diffuse
    float diff = max(dot(-lightDirection, N), 0.0);
    vec3 diffuse = diff * diffuseReflectance * lightColour;

    return (ambient + diffuse);
}

void main(void) {
    gl_Position = projectionMatrix * viewMatrix * vec4(in_position, 1.0);

    gl_ClipDistance[0] = dot(vec4(in_position, 1.0), waterClippingPlane); // test for clipping

    vec3 lighting = getLight();
    out_colour = lighting * in_colour;
}

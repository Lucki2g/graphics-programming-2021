#version 400 core

in vec3 outColor;

void main(void) {
    gl_FragColor = vec4(outColor, 1.0);
}

#version 330 core
layout (location = 0) in vec2 pos;   // the position variable has attribute position 0
// TODO 2.2 add velocity and timeOfBirth as vertex attributes
layout (location = 1) in vec2 vel;
layout (location = 2) in float tob;

// TODO 2.3 create and use a float uniform for currentTime
uniform float currentTime;

// TODO 2.6 create out variable to send the age of the particle to the fragment shader
out float Age;

void main()
{
    // TODO 2.3 use the currentTime to control the particle in different stages of its lifetime
    float age = currentTime - tob;
    if (tob == 0 || age > 10.0f) gl_Position = vec4(0f, 0f, 0f, 1.0f);
    else {
        vec2 position = pos + age * vel;
        // TODO 2.6 send the age of the particle to the fragment shader using the out variable you have created
        // this is the output position and and point size (this time we are rendering points, instad of triangles!)
        gl_Position = vec4(position, 0.0, 1.0);
        gl_PointSize = mix(0.1f, 20.0f, (age / 10.0f));
        Age = age;
    }
}
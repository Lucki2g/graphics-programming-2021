#version 330 core

out vec4 fragColor;

// TODO 2.6: should receive the age of the particle as an input variable
in float Age;

void main()
{
    // TODO 2.4 set the alpha value to 0.2 (alpha is the 4th value of the output color)
    // fragColor = vec4(1.0, 1.0, 1.0, 0.2f);

    // TODO 2.5 and 2.6: improve the particles appearance
    float distance = distance(gl_PointCoord, vec2(0.5f, 0.5f));
    float alpha = (0.5f - distance) * (1.0f - Age / 10.0f);
    float g = mix(1.0f, 0.0f, alpha);
    float b = mix(0.05f, 0.0f, pow(alpha, 2));

    fragColor = vec4(1.0f, g, b, alpha);
}
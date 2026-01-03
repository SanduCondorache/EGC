#version 330 core

in vec3 frag_color;
out vec4 out_color;

uniform float uTime;
uniform float uMaxTime;

void main()
{
    float t = clamp(uTime / uMaxTime, 0.0, 1.0);
    vec3 finalColor = mix(frag_color, vec3(1.0, 0.0, 0.0), t);
    out_color = vec4(finalColor, 1.0);
}

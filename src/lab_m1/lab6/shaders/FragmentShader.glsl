#version 330

// Input
// TODO(student): Get values from vertex shader

// Output
layout(location = 0) out vec4 out_color;

in vec3 vPos;

uniform float time;

void main()
{
    float speed = 5.0;

    float pat = sin(vPos.x * speed + 2 * time) + cos(vPos.x * speed + 10 * time) * time;
    // float pat = sin(vPos.x  * speed);

    vec3 col1 = vec3(1, 0, 0);
    vec3 col2 = vec3(1, 1, 1);

    vec3 color = mix(col1, col2, pat);

    out_color = vec4(color, 1);

}

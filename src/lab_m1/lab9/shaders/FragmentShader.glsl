#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
// TODO(student): Declare various other uniforms
uniform int two;
uniform float time;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture2D() function.
    vec4 color1 = texture2D(texture_1, texcoord);
    
    if (color1.a < 0.5f) {
        discard;
    }

    if (time >= 0.0f && two == 2) {
        vec4 color2 = texture2D(texture_2, texcoord);
        out_color = mix(color1, color2, sin(abs(time)) + 0.5);
    } else if (two == 1) {
        vec4 color2 = texture2D(texture_2, texcoord);
        out_color = mix(color1, color2, 0.5f);
    } else {
        out_color = color1;
    }


}

#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 frag_color;

void main()
{
    gl_Position = Projection * View * Model * vec4(aPos, 1.0);
    frag_color = aColor;
}

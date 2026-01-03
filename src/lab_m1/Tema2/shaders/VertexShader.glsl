#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 vertex_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 frag_color;

void main()
{
    frag_color = vertex_color;
    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}

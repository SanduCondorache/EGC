#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 vertex_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 vPos;

void main()
{
    vPos = v_position;

    gl_Position = Projection * View * Model * vec4(v_position, 1.0);

}

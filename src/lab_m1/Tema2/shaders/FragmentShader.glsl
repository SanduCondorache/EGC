#version 330

in vec3 frag_color;

layout(location = 0) out vec4 out_color;


void main() {
    if (distance(frag_color, vec3(0.0, 0.3, 1.0)) < 0.001) {
        out_color = vec4(frag_color * 0.5, 0.5);
    } else {
        out_color = vec4(frag_color, 1);
    }
}

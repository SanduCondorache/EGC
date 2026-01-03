#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 light_direction1;
uniform vec3 light_position1;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms
uniform int spotlight;

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;

vec3 point_light_contribution(vec3 light_pos, vec3 light_color, float intensity)
{
    vec3 n = normalize(world_normal);
    vec3 f = world_position;
    vec3 l = normalize(light_pos - f);
    vec3 v = normalize(eye_position - f);
    vec3 h = normalize(l + v);
    vec3 color;
    float ambient_light = 0.25;
    float diffuse_light = material_kd * max(dot(n, l), 0);
    float specular_light = 0;
    if (diffuse_light > 0) {
        vec3 r = reflect(-l, n);
        specular_light = material_ks * pow(max(dot(v, r), 0), float(material_shininess));
    }




    float light = ambient_light + intensity * (diffuse_light + specular_light);

    color = light * light_color;
    return color;
}

vec3 func(vec3 light_pos, vec3 light_color) {
    vec3 n = normalize(world_normal);
    vec3 f = world_position;
    vec3 l = normalize(f - light_pos);
    vec3 v = normalize(eye_position - f);
    vec3 r = reflect(-l, n);

    float cutOff = cos(radians(30.0));
    float outerCutOff = cos(radians(35.0));

    float theta = dot(l, normalize(light_direction)); 
    float intensity = smoothstep(outerCutOff, cutOff, theta);

    float ambient  = 0.25;

    float diffuse  = material_kd * max(dot(n, -l), 0.0);
    float specular = 0.0;

    if (diffuse > 0.0) {
        specular = material_ks * pow(max(dot(v, r), 0.0), float(material_shininess));
    }

    float light = ambient + intensity * (diffuse + specular);

    return (object_color * light);
}


void main()
{
    vec3 n = normalize(world_normal);
    vec3 f = world_position;
    vec3 l = normalize(light_position - f);
    vec3 v = normalize(eye_position - f);
    vec3 h = normalize(l + v);
    if (spotlight == 0) {
        vec3 color1 = point_light_contribution(light_position, vec3(1.0), 1);
        vec3 color2 = point_light_contribution(light_position1, vec3(1.0), 1);
        float view_factor = 1.0 - max(0.0, dot(normalize(world_normal), normalize(eye_position - world_position)));
        vec3 c12 = mix(vec3(0.7, 0.1, 0.9), vec3(0.0, 1.0, 0.6), smoothstep(0.0, 0.5, view_factor));
        vec3 c23 = mix(vec3(0.0, 1.0, 0.6), vec3(0.0, 0.2, 1.0), smoothstep(0.5, 1.0, view_factor));
        vec3 chroma = mix(c12, c23, view_factor);
        out_color = vec4(chroma * object_color * (color1 + color2), 1.0);
    } else {

        vec3 color1 = func(light_position, vec3(1.0));
        vec3 color2 = func(light_position1, vec3(1.0));
        out_color = vec4(object_color * (color1 + color2), 1.0);
    }
}

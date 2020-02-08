#version 330 core

layout (location = 0) in vec3 v;
layout (location = 1) in vec3 v2;
layout (location = 2) in vec3 nv;
layout (location = 3) in vec3 nA;
layout (location = 4) in vec3 nB;
layout (location = 5) in int kind;

out vec3 colour;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec2 screen(mat4 M, vec3 vector){
    vec4 proj_vector = M * vec4(vector, 1.0);
    return vec2(proj_vector.x/proj_vector.w, proj_vector.y/proj_vector.w);
}

void main()
{
    // figure out if this needs to be drawn

    // find transformation matrix
    mat4 M = projection * view * model;
    vec4 pos = (M * vec4(v, 1.0));

    // calculate witdh multiplier
    float w = 0.02;
    // ...

    // calculate 2D coordinates
    vec2 s  = screen(M, v);
    vec2 s2 = screen(M, v2);
    vec2 ns = screen(M, v+w*nv);

    vec2 p  = normalize(vec2(s.y-s2.y, s2.x-s.x));
    vec2 m  = normalize(ns-s);

    switch(kind){
        case 0:
            gl_Position = pos+vec4(w*p, 0.0, 0.0);
            colour = vec3(0.1, 0.1, 0.1); break;
        case 1:
            gl_Position = pos-vec4(w*p, 0.0, 0.0);
            colour = vec3(0.1, 0.1, 0.1); break;
        case 2:
            gl_Position = pos+vec4(w*m, 0.0, 0.0);
            colour = vec3(0.9, 0.1, 0.1); break;
        default:
            gl_Position = pos;
            colour = vec3(0.0, 0.0, 0.0); break;
    }
}

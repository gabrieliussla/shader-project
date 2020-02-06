#version 330 core

layout (location = 0) in vec3 v;
layout (location = 1) in vec3 nv;
layout (location = 2) in vec3 nA;
layout (location = 3) in vec3 nB;
layout (location = 4) in int kind;

out vec3 colour;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec2 screen(mat4 projection, vec3 vector){
    vec4 proj_vector = projection * vec4(vector, 1.0);
    return vec2(proj_vector.x/proj_vector.w, proj_vector.y/proj_vector.w);
}

void main()
{
    // find transformation matrix
    mat4 M = projection * view * model;
    //M = mat4(1.0);
    float depth = (M * vec4(v, 1.0)).w;

    // calculate witdh multiplier
    float width = 2.0;
    // ...

    // calculate 2D coordinates
    vec2 s = screen(M, v);                  // screen coordinate of vertex
    vec2 m = normalize(screen(M, nv));      // vertex normal
    vec2 p = normalize(screen(M, nA+nB));   // perpendicular to edge

    switch(kind){
        case 0:
            gl_Position = vec4(s+0.01*p, 0.5, 1.0);
            colour = vec3(1.0, 0.0, 0.0); break;
        case 1:
            gl_Position = vec4(s-0.01*p, 0.5, 1.0);
            colour = vec3(0.0, 1.0, 0.0); break;
        case 2:
            gl_Position = vec4(s+0.01*m, 0.5, 1.0);
            colour = vec3(0.0, 0.0, 1.0); break;
        default:
            gl_Position = vec4(0.0, 0.0, 0.0, 0.0);
            colour = vec3(0.8, 0.8, 0.8); break;
    }
}

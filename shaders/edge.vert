#version 330 core

layout (location = 0) in vec3 v;
layout (location = 1) in vec3 v2;
layout (location = 2) in vec3 nv;
layout (location = 3) in vec3 nA;
layout (location = 4) in vec3 nB;
layout (location = 5) in int kind;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 eye;
uniform vec3 light;

out vec4 colour;

vec2 screen(mat4 M, vec3 vector){
    vec4 proj_vector = M * vec4(vector, 1.0);
    return vec2(proj_vector.x/proj_vector.w, proj_vector.y/proj_vector.w);
}

void main()
{
    // find transformation matrix
    mat4 M = projection * view * model;
    vec4 pos = (M * vec4(v, 1.0));

    // calculate witdh multiplier
    vec3 lightVec = normalize(vec4(v, 1.0).xyz-light);
    float vW = 0.05 * length(nv+lightVec) - 0.00;
    // extra value needed for sharp objects
    vec3 edgeNormal = normalize(nA+nB);
    float eW = 0.04 * length(edgeNormal+lightVec) + 0.03;
    // value needed for tangent-based lighting
    vec3 tangent = normalize(cross(eye-v, v2-v));
    if(dot(tangent, nv) < 0)
        tangent = -tangent;
    float tW = 0.05 * length(tangent+lightVec) - 0.00;

    // make sure variables don't go negative
    if(vW < 0)
        vW = 0;


//if(kind == 2){
//    pos = (M*(vec4(v, 1.0)+0.1*vec4(tangent, 0.0)));
//}

    // calculate 2D coordinates
    vec2 s  = screen(M, v);
    vec2 s2 = screen(M, v2);
    vec2 ns = screen(M, v+vW*nv);

    vec2 p  = normalize(vec2(s.y-s2.y, s2.x-s.x));
    vec2 m  = normalize(ns-s);
    if(dot(p, m) < 0)
        p = -p;

    switch(kind){
        case 0:
            gl_Position = pos;
            colour = vec4(0.0, 0.0, 0.0, 1.0); break;
        case 1:
            gl_Position = pos+vec4(vW*p, 0.0, 0.0);
            colour = vec4(0.0, 0.0, 0.0, 1.0); break;
        case 2:
            gl_Position = pos+vec4(vW*m, 0.0, 0.0);
            colour = vec4(1.0, 0.0, 0.0, 1.0); break;
        default:
            gl_Position = pos;
            colour = vec4(0.0, 0.0, 0.0, 1.0); break;
    }
    // figure out if it needs to be drawn
    vec4 gv = model * vec4(v, 1.0);
    vec4 gnA = model * vec4(nA, 0.0);
    vec4 gnB = model * vec4(nB, 0.0);
    if(dot(gnA,vec4(eye, 1.0)-gv) < 0 == dot(gnB,vec4(eye, 1.0)-gv) < 0)
        //colour = vec4(0.3, 0.3, 0.6, 0.05);
        gl_Position = vec4(0.0, 0.0, 0.0, 0.0);
}

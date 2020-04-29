#version 330 core

#define SCALE 0.05
#define REDUCE -0.01

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
uniform float ratio;

out vec4 colour;

vec2 screen(mat4 M, vec3 vector){
    vec4 proj_vector = M * vec4(vector, 1.0);
    return vec2(proj_vector.x/proj_vector.w, proj_vector.y/proj_vector.w);
}

void main()
{
    // figure out if it needs to be drawn
    vec4 gv  = model * vec4(v, 1.0);
    vec4 gnA = model * vec4(nA, 0.0);
    vec4 gnB = model * vec4(nB, 0.0);

    mat4 M = projection * view * model;
    vec4 pos = (M * vec4(v, 1.0));
    
    // calculate witdh multiplier
    vec3 vertex        = (model*vec4(v,  1.0)).xyz;
    vec3 vertex2       = (model*vec4(v2, 1.0)).xyz;
    vec3 lightVec      = normalize(vertex-light);
    vec3 edgeNormal    = normalize(nA+nB);
    vec3 edgeTangent   = normalize(cross(vertex-eye, vertex-vertex2));
     if(dot(edgeTangent, nv) < 0) edgeTangent = -edgeTangent;
    vec3 vertexTangent = normalize(nv*(dot(vertex-eye, vertex-eye)) - (vertex-eye)*dot(vertex-eye, nv));
    
    // edge-geometry lighting
    float eW = SCALE * length(edgeNormal+lightVec) - REDUCE;
    if(eW < 0) eW = 0;
    // vertex-geometry lighting
    float vW = SCALE * length(nv+lightVec) - REDUCE;
    if(vW < 0) vW = 0;
    // edge-tangent lighting
    float teW = SCALE * length(edgeTangent+lightVec) - REDUCE;
    if(teW < 0) teW = 0;
    // vertex-tangent lighting
    float tvW = SCALE * length(vertexTangent+lightVec) - REDUCE;
    if(tvW < 0) tvW = 0;
    //vW = 0.05;
    
    // calculate 2D coordinates and vectors
    vec2 s  = screen(M, v);
    vec2 s2 = screen(M, v2);
    vec2 ns = screen(M, v+vW*nv);
    
    s.x  *= ratio;
    s2.x *= ratio;
    ns.x *= ratio;
    
    vec2 p  = normalize(vec2(s.y-s2.y, (s2.x-s.x)));
    vec2 m  = normalize(ns-s);
    
    p.x /= ratio;
    m.x /= ratio;
    
    if(dot(p, m) < 0) p = -p;
    
    // set position
    switch(kind){
        case 0:
            gl_Position = pos;
            //gl_Position = pos-vec4(vW*p, 0.0, 0.0);
            colour = vec4(0.0, 0.0, 0.0, 1.0); break;
        case 1:
            gl_Position = pos+vec4(vW*m, 0.0, 0.0);
            colour = vec4(0.0, 0.0, 0.0, 1.0); break;
        case 2:
            gl_Position = pos+vec4(vW*m, 0.0, 0.0);
            colour = vec4(0.0, 0.0, 0.0, 1.0); break;
        default:
            gl_Position = pos;
            colour = vec4(0.0, 1.0, 1.0, 1.0); break;
    }
    //colour = vec4(0.3, 0.3, 0.3, 1.0);

    if(dot(gnA,vec4(eye, 1.0)-gv) < 0 == dot(gnB,vec4(eye, 1.0)-gv) < 0 && dot(normalize(nA), normalize(nB)) > -0.2)
        //colour = vec4(1.0, 0.0, (dot(nA,nB)+2)/4, 1.0);
        gl_Position = vec4(0.0, 0.0, 0.0, 0.0);
}

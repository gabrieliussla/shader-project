#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 vNormal;
  
out vec3 colour;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 pos = (projection * view * model * vec4(aPos, 1.0));
    gl_Position = vec4(pos.xy, 0.999*pos.w, pos.w);

    vec4 globalNormal = model * vec4(vNormal, 0.0);
    colour = vec3(0.8, 0.8, 0.9);
}

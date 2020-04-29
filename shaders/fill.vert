#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 vNormal;
  
out vec3 colour;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 eye;

uniform vec3 light;
uniform vec3 shade;

void main()
{
    gl_Position = (projection * view * model * vec4(aPos, 1.0));

    vec4 globalNormal = model * vec4(vNormal, 0.0);
    vec4 globalPos    = model * vec4(aPos, 1.0);
    float val = 0.55 + 0.075 * length(eye-globalPos.xyz);//dot(normalize(globalNormal).xyz, normalize(light));
    colour = vec3(val, val, val);
    colour = shade;
}

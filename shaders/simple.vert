#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 vNormal;
  
out vec3 colour;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 light;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    vec4 globalNormal = model * vec4(vNormal, 0.0);
    float shade = 0.5;
    shade = 0.3 + dot(globalNormal, vec4(normalize(light - aPos), 0.0)) / 5;
    colour = vec3(shade, shade, shade);
}

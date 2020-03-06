#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in float lightness;
layout (location = 3) in int kind;
  
out vec4 colour;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 pos = projection * view * model * vec4(position, 1.0);
    vec4 globalNormal = model * vec4(normal, 0.0);
    float size = 0.29;

    switch(kind){
        case 0:
            pos += vec4(-size, size, 0, 0); break;
        case 1:
            pos += vec4(size, size, 0, 0); break;
        case 2:
            pos += vec4(-size, -size, 0, 0); break;
        case 3:
            pos += vec4(size, -size, 0, 0); break;
        default:
            pos = vec4(0, 0, 0, 1);
    }
    pos.z = -1*pos.w;
    gl_Position = pos;
    colour = vec4(lightness, 0.2, 0.2, 1);
}

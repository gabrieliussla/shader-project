#version 330 core

#define TEX_SIZE 0.7
#define STRENGTH 0.4
#define FAR_VAL 4.0

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in float lightness;
layout (location = 3) in int kind;
  
out vec4 colour;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 eye;

void main()
{
    vec4 globalNormal = model * vec4(normal, 0.0);
    vec4 globalPos = model * vec4(position, 1.0);
    vec4 pos = projection * view * globalPos;

    switch(kind){
        case 0:
            pos += vec4(-TEX_SIZE, TEX_SIZE, 0, 0);
            texCoord = vec2(0, 1); break;
        case 1:
            pos += vec4(TEX_SIZE, TEX_SIZE, 0, 0);
            texCoord = vec2(1, 1); break;
        case 2:
            pos += vec4(-TEX_SIZE, -TEX_SIZE, 0, 0);
            texCoord = vec2(0, 0); break;
        case 3:
            pos += vec4(TEX_SIZE, -TEX_SIZE, 0, 0);
            texCoord = vec2(1, 0); break;
        default:
            pos = vec4(0, 0, 0, 1);
            texCoord = vec2(0, 0);
    }
    //pos.z = -pos.w; //bring to front
    gl_Position = pos;
    vec3 toPoint = eye-(globalPos.xyz/globalPos.w);
    float distanceFactor = FAR_VAL / (FAR_VAL + length(toPoint));
    colour = vec4(lightness, lightness, lightness, STRENGTH*distanceFactor*dot(normalize(toPoint), normal));
}

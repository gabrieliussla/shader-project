#version 330 core

layout (location=0) in vec2 pos;
layout (location=1) in vec2 tex;

out vec2 texCoord;
out vec2 imageCoord;
out vec3 lightPos;

uniform vec3 eye;
uniform vec3 flatlight;
uniform float ratio;

void main(){
    gl_Position = vec4(pos, 0.0, 1.0);
    texCoord = tex;
    imageCoord = tex;
    if(ratio > 1.0) imageCoord.y /= ratio;
    else            imageCoord.x *= ratio;
    lightPos = flatlight;
}

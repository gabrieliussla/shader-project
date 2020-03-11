#version 330 core

out vec4 FragColour;

in vec4 colour;
in vec2 texCoord;

uniform sampler2D image;

void main(){
    FragColour = vec4(colour.xyz, colour.w*texture(image, texCoord).w);
}

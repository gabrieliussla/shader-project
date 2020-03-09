#version 330 core

out vec4 FragColour;

in vec4 colour;
in vec2 texCoord;

uniform sampler2D image;

void main(){
    FragColour = texture(image, texCoord);
}

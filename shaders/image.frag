#version 330 core

#define SAMPLES 8

out vec4 fragColour;

in vec2 texCoord;

uniform sampler2D screenTexture;

void main(){
    fragColour = texture(screenTexture, texCoord);
}

#version 330 core

#define SAMPLES 8

out vec4 fragColour;

in vec2 texCoord;
in vec2 imageCoord;
in vec3 lightPos;

uniform sampler2D screenTexture;
uniform sampler2D paper;

void main(){
    vec3 toLight = normalize(lightPos - vec3(texCoord, 0.0));
    vec3 paperNormal = texture(paper, imageCoord).xyz - 0.5;
    fragColour = vec4(dot(paperNormal, toLight) * texture(screenTexture, texCoord).xyz, 1.0);
}

#version 330 core

#define WOBBLE 0.01

out vec4 fragColour;

in vec2 texCoord;   // rendered screen
in vec2 imageCoord; // added texture
in vec3 lightPos;   // in screen coordinates, truncated

uniform sampler2D screenTexture;
uniform sampler2D paper;
//uniform sampler2D wobble;

void main(){
    vec2 wobbleCoord = texCoord + WOBBLE * (texture(paper, imageCoord/5).xy - 0.5);
    vec4 colour = texture(screenTexture, wobbleCoord);

    vec3 toLight = normalize(lightPos - vec3(texCoord, 0.0));
    vec3 paperNormal = texture(paper, imageCoord/2).xyz - 0.5;

    fragColour = vec4(dot(paperNormal, toLight) * colour.xyz, 1.0);
}

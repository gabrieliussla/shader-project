#version 330 core

out vec4 FragColour;

in vec4 colour;

void main(){
  FragColour = colour;
}

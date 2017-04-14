#version 330 core

out vec4 color;
in vec2 TexCoords;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300;  

void main()
{
    color = texture(screenTexture, TexCoords);
}
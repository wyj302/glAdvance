#version 330 core 

out vec4 color;
in vec2 TexCoords;

uniform sampler2D texture0;

void main()
{
	color = vec4(texture(texture0, TexCoords));
}
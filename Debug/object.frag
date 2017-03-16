#version 330 core
out vec4 color;

uniform vec3 objectColor;
uniform vec3 lampColor;

void main()
{
	color = vec4(objectColor * lampColor, 1.0f);
}
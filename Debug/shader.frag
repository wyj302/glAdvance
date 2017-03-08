#version 330 core

in vec2 TexCoord;
in vec3 ourColor;
uniform sampler2D texture0;
uniform sampler2D texture1;
out vec4 color;
 
void main()
{
	color = mix(texture(texture0,  TexCoord), texture(texture1, 1 - TexCoord), 0.2) ;
}
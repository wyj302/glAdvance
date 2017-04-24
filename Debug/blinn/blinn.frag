#version 330 core

out vec4 FragColor;

in VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
}fs_in;

uniform bool blinn;
uniform vec3 viewPos;
uniform vec3 lightPos;

uniform sampler2D floorTexture;


void main()
{
	vec3 color = texture(floorTexture, fs_in.TexCoords).rgb;

	//ambient
	vec3 ambient = color * 0.5;

	//diffuse
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 normal = normalize(fs_in.Normal);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * color;

	//specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = 0.0;
	if(blinn)
	{
		vec3 halfwayDir = normalize(lightDir + viewDir);
		spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
	}
	else
	{
	    vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
	}
	vec3 specular = spec * vec3(0.3);

	FragColor = vec4(ambient + diffuse + specular, 1.0);
}
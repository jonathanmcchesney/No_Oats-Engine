#version 330 core
in vec2 TexCoords;
out vec4 color;
uniform sampler2D texture_diffuse;
void main()
{
	vec3 diffuse = texture(texture_diffuse, TexCoords).rgb;
	color = vec4(diffuse, 1.0f);
}
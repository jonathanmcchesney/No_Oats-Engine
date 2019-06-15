#version 330 core

// Material Struct
struct Material
{
	sampler2D diffuse, specular;
	float shininess;
};

// Light Struct
struct Light
{
	vec3 position, ambient, diffuse, specular;
};

// Inputs
in vec3 FragPos, Normal;
in vec2 TexCoords;

// Outputs
out vec4 color;

// Uniforms
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
	//ambient Lighting
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	//diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	//specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	color = vec4(ambient + diffuse + specular, 1.0f);
}

#version 330 core

in vec2 TexCoord; 
in vec3 FragPos; 
in vec3 Normal;

out vec4 frag_color;

uniform sampler2D texSampler1;
uniform vec3 lightPos; 
uniform vec3 lightColor; 
uniform vec3 viewPos; 
uniform vec4 vertColor;
uniform float ambientIntensity;
uniform vec4 transparancy;



void main()
{
	//we get the equations of ambient, diffuse and specular from the slides
	

	//Ambient
	float ambientFactor = 0.9f;
	//vec3 ambient = lightColor * ambientFactor;

	vec3 ambient = lightColor * ambientFactor * ambientIntensity;

	//Diffuse
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float NDotL = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = lightColor * NDotL;

	//specular
	float speculatFactor = 0.3f; 
	float shininess = 32.0f; 
	vec3 viewDir = normalize(viewPos - FragPos); 
	

	//Blinn-Phong:
	
	vec3 halfDir = normalize(lightDir + viewDir); 
	float NDotH = max(dot(normal, halfDir), 0.0f);
	vec3 specular = lightColor * speculatFactor * pow(NDotH, shininess);
	
	 vec4 texel = texture(texSampler1, TexCoord);
	 frag_color = vec4(ambient+diffuse+specular, 1.0f)* texel*vertColor*transparancy;
};

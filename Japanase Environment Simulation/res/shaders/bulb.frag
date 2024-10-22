
#version 330 core

out vec4 frag_color; 
uniform vec3 lightColor; 
//uniform float bulbIntensity;


void main()
{

	//frag_color = vec4(lightColor * bulbIntensity, 1.0f); 

	 frag_color = vec4(lightColor, 1.0f); 
};

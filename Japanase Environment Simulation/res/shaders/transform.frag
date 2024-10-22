
#version 330 core
out vec4 frag_color;
in vec2 TexCoord;

uniform vec4 vertColor;
uniform sampler2D texSampler1;
uniform sampler2D texSampler2;


void main()
{
	 vec4 brickColor = texture(texSampler1, TexCoord) ;
	 vec4 mossColor = texture(texSampler2, TexCoord);
	frag_color = vec4(mix(brickColor.rgb, mossColor.rgb,  mossColor.a), 1.0) * vertColor;

};


    #version 330 core
	layout (location = 0) in vec3 pos;
	layout (location = 2) in vec3 normal; 
	layout (location = 1) in vec2 texCoord; //for texture coordinates 
	
	out vec2 TexCoord;
	out vec3 FragPos; 
	out vec3 Normal;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;
	void main()
	{
		FragPos = vec3(model * vec4(pos, 1.0f));
		//Normal = normal; //basic
		Normal = mat3(transpose(inverse(model))) * normal;
		TexCoord = texCoord;
	    gl_Position = projection * view * model * vec4(pos.x, pos.y, pos.z, 1.0);
	
	};

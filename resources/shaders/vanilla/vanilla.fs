#version 430
in vec3 vposition;
in vec2 vtexcoord;

uniform sampler2D diffuse_texture;
uniform sampler2D normal_texture;


layout (location = 0)out vec4 FragColor;
layout (location = 1)out vec4 redColor;
void main()
{
    FragColor = texture(diffuse_texture,vtexcoord);
	//FragColor = gl_FragColor;
	redColor = vec4(1.0,0.0,0.0,1.0);

}
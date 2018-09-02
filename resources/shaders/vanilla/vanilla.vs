#version 430
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 Texcoord;
layout (location = 2) in vec3 Normal;
layout (location = 3) in vec3 Tangent;
uniform mat4 MVP;

out vec3 vposition;
out vec2 vtexcoord;

void main()
{
    gl_Position = MVP * vec4(Position, 1.0);
    vposition = Position;
	vtexcoord = Texcoord;
}
#version 400

layout(location = 0) in vec4 position;

smooth out vec3 texCoord;

uniform mat4 ProjectionViewMatrix;

void main()
{
	texCoord = position.xyz;
	gl_Position = ProjectionViewMatrix * position;
}

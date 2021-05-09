#version 400

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 uvCoord;

uniform mat4 ProjectionViewMatrix;
uniform mat4 ModelMatrix;

smooth out vec4 vertPos;
smooth out vec4 vertNormal;
smooth out vec2 vertUV;

void main()
{
	vertUV = uvCoord;
	vertNormal = normal;
	vertPos = ModelMatrix * position;
	gl_Position = ProjectionViewMatrix * position;
}

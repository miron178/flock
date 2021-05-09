#version 400

smooth in vec3 texCoord;

out vec4 outputColour;

uniform samplerCube CubeMapTexture;

void main()
{
	//outputColour = vec4(texCoord, 1.0f);
	outputColour = texture(CubeMapTexture, texCoord);
}

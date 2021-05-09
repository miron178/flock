#version 400

smooth in vec4 vertPos;
smooth in vec4 vertNormal;
smooth in vec2 vertUV;

out vec4 outputColour;

uniform vec4 camPos;
uniform vec4 kA;
uniform vec4 kD;
uniform vec4 kS;
 
uniform sampler2D DiffuseTexture;
uniform sampler2D SpecularTexture;
uniform sampler2D NormalTexture;

vec3 iA = vec3(.25f, .25f, .25f);
vec3 iD = vec3(1.f, 1.f, 1.f);
vec3 iS = vec3(1.f, 1.f, 1.f);

vec4 lightDir = normalize(vec4(0.f) - vec4(10.f, 8.f, 10.f, 0.f));

void main()
{
	vec4 diffuseData  = texture(DiffuseTexture, vertUV);
	vec4 specularData = texture(SpecularTexture, vertUV);
	vec4 normalData   = texture(NormalTexture, vertUV);

    vec4 normal = vec4(normalize(normalData.rgb * 2.0f - 1.0f), 1.0f);
	//vec4 normal = normalize(vertNormal);

	vec3 Ambient = kA.xyz * iA;

	float nDl = max(0.f, dot(normal, -lightDir));
	vec3 Diffuse = kD. xyz * iD * nDl * diffuseData.rgb;

	vec3 R = reflect(lightDir, normal).xyz;
	vec3 E = normalize(camPos - vertPos).xyz;

	float specTerm = pow(max(0.f, dot(E, R)), kS.a);
	vec3 Specular = kS.xyz * iS * specTerm * specularData.rgb;

	outputColour = vec4(Ambient + Diffuse + Specular, 1.f);
	//outputColour = vec4(Ambient, 1.f);
	//outputColour = vec4(Diffuse, 1.f);
	//outputColour = vec4(Specular, 1.f);
}

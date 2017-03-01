#version 330

uniform mat4 MVPmatrix;
uniform mat4 modelMatrix;
uniform float texScale;
uniform float time;
uniform int random1 [15];
uniform float random2 [15];
uniform float glitchIntensity;
in vec3 vertex;
in vec3 normal;
in vec2 uv;
out vec4 fragWorldPosition;
out vec4 fragWorldNormal;
out vec2 fragUV;

void main (void)
{
	fragWorldNormal = normalize (modelMatrix * vec4 (normal, 0.0f));
	int index1 = int(round ((vertex.x * random2 [6]) + ((vertex.y * random2 [4]) - (vertex.z * random2 [8]))) * 100)%15;
	int index2 = int(round (((vertex.x * random2 [7]) - (vertex.y * random2 [3])) + (vertex.z * random2 [9])) * 100)%15;
	fragUV = uv * texScale;
	vec2 center = vec2 (0.5f, 0.5f);
	fragUV = (fragUV - center) * (1 + sin (time * random2 [random1 [index1]] + length (vertex) * 2) * glitchIntensity) + center;

	float tempScale = 1;
	if (index2 != random1 [index1] && index2 != random1 [index2] && index1 != random1 [index2] && index1 != random1 [index1] && index2 == index1)
	{
		tempScale = random2 [random1 [index2]] * glitchIntensity + (1 - glitchIntensity);
	}
	fragWorldPosition = modelMatrix * vec4 (vertex * tempScale, 1.0f);
	gl_Position = MVPmatrix * vec4 (vertex * tempScale, 1.0f);
}
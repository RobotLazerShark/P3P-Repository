#version 330

uniform mat4 MVPmatrix;
uniform mat4 modelMatrix;
uniform float texScale;
in vec3 vertex;
in vec3 normal;
in vec2 uv;
out vec4 fragWorldPosition;
out vec4 fragWorldNormal;
out vec2 fragUV;

void main (void)
{
	fragWorldNormal = normalize (modelMatrix * vec4 (normal, 0));
	gl_Position = MVPmatrix * vec4 (vertex, 1);
	fragUV = uv * texScale;
	fragWorldPosition = modelMatrix * vec4 (vertex, 1);
}
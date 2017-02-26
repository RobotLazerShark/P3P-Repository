#version 330

uniform mat4 MVPmatrix;
uniform float time;
in vec3 vertex;
in vec3 normal;
in vec2 uv;
out vec2 fragUV;
out vec3 fragNormal;

void main (void)
{
	fragNormal = normal;
	float intensity = 0.03f;
	float tempScale = (sin (time * 2.5f + (length (vertex) * 0.5f)) + 1) * intensity + (1 - intensity);
	gl_Position = MVPmatrix * vec4 (vertex * tempScale, 1.0f);
	fragUV = uv;
}
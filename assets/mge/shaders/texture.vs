#version 330
uniform mat4 MVPmatrix;
uniform float scale;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec2 texCoord;

void main (void)
{
	gl_Position = MVPmatrix * vec4 (vertex, 1);
	texCoord = uv * scale;
}
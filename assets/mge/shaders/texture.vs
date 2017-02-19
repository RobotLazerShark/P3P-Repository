#version 330
uniform mat4 MVPmatrix;
uniform float scale;
uniform float time;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec2 texCoord;

void main (void)
{
	gl_Position = MVPmatrix * vec4 (vertex,1);//.x, vertex.y + (sin (time * 2 + length (vertex) * 2) * 0.05f + 0.05f), vertex.z, 1);
	texCoord = uv * scale;
	if (time > 0)
	{
		vec2 center = vec2 (0.5f, 0.5f);
		texCoord = (texCoord - center) * (1 + sin (time * 2 + length (vertex) * 2) * 0.05f) + center;
	}
}
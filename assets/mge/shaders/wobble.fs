#version 330

uniform sampler2D texture;
in vec2 fragUV;
out vec4 fragment_color;

void main (void)
{
	fragment_color = texture (texture, fragUV);
}

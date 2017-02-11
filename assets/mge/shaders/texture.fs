#version 330

uniform sampler2D textureDiffuse;
in vec2 texCoord;
out vec4 fragment_color;

void main( void ) {
	fragment_color = texture2D (textureDiffuse,texCoord);
}

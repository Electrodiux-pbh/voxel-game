#version 410 core

in vec2 _texture_coord;

uniform sampler2D _texture;

out vec4 color;

void main()
{
	vec4 texture_color = texture(_texture, _texture_coord);
	if(texture_color.a < 0.1)
		discard;
	color = texture_color;
}
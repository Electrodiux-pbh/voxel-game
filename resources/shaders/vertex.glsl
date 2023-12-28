#version 410 core

layout (location = 0) in vec3 l_position;
layout (location = 1) in vec2 l_texture_coord;

uniform mat4 projection;
uniform mat4 view;

out vec2 _texture_coord;

void main()
{
	gl_Position = projection * view * vec4(l_position, 1.0);
	_texture_coord = l_texture_coord;
}
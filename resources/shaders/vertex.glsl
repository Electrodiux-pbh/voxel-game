#version 410 core

layout (location = 0) in vec3 __position;
layout (location = 1) in vec2 __texture_coord;

uniform mat4 projection;
uniform mat4 view;

out vec2 _texture_coord;

void main()
{
	gl_Position = projection * view * vec4(__position, 1.0);
	_texture_coord = __texture_coord;
}
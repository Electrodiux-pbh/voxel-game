#version 410 core

layout (location = 0) in vec3 __position;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;

void main()
{
	gl_Position = projection * view * transform * vec4(__position, 1.0);
}
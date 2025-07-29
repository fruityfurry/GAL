#version 430 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 vTexCoord;

out vec2 fTexCoord;

uniform float t;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0) + vec4(0.1 * sin(2.0 * t), 0.0, 0.0, 1.0);
	fTexCoord = vTexCoord;
}
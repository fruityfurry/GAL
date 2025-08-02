#version 430 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 vTexCoord;

out vec2 fTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec3 newPos = normalize(pos);
	gl_Position = projection * view * model * vec4(newPos, 1.0);
	fTexCoord = vTexCoord;
}
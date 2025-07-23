#version 430 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec2 vTexCoord;

out vec3 fColor;
out vec2 fTexCoord;

uniform float t;

void main()
{
	gl_Position = vec4(pos + vec3(0.1 * sin(2.0 * t), 0.0, 0.0), 1.0);
	fColor = vColor;
	fTexCoord = vTexCoord;
}
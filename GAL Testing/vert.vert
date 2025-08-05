#version 430 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

out vec3 fPos;
out vec3 fNormal;
out vec2 fTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(vPos, 1.0);
	fPos = vec3(model * vec4(vPos, 1.0));

	fNormal = mat3(transpose(inverse(model))) * vNormal;
	fTexCoord = vTexCoord;
}
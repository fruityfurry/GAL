#version 430 core
out vec4 FragColor;

in vec3 fColor;
in vec2 fTexCoord;

uniform sampler2D texture1;
uniform vec4 transparentColor;

void main()
{
	vec4 texColor = texture(texture1, fTexCoord);

	if (texColor.a == 0.0)
		FragColor = transparentColor;
	else
		FragColor = mix(vec4(fColor, 1.0), texColor, 0.5);
}
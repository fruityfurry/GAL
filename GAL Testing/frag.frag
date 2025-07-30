#version 430 core
out vec4 FragColor;

in vec2 fTexCoord;

uniform sampler2D texture1;
uniform vec4 hue;
uniform float hueStrength;

void main()
{
	vec4 texColor = texture(texture1, fTexCoord);

	if (texColor.a == 0.0)
		discard;
	else
		FragColor = mix(texColor, hue, hueStrength);
}
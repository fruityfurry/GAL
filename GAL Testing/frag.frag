#version 430 core
out vec4 FragColor;

in vec3 fPos;
in vec3 fNormal;
in vec2 fTexCoord;

uniform sampler2D texture1;

uniform float ambientLight;

uniform vec3 lightPos;
uniform vec3 lightColor;

void main()
{
	vec4 texColor = texture(texture1, fTexCoord);

	if (texColor.a == 0.0)
		discard;
	else
	{
		vec3 lightDir = normalize(lightPos - fPos);
		vec3 diffuseLight = max(dot(normalize(fNormal), lightDir), 0.0) * lightColor;

		FragColor = vec4(ambientLight + diffuseLight, 1.0) * texColor;
	}
}
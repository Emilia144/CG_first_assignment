#version 330 core
in vec3 Color;
in vec2 TexCoord;

out vec4 outColor;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;

void main()
{
	vec4 colTex1 = texture(ourTexture, TexCoord);
	vec4 colTex2 = texture(ourTexture2, TexCoord);
	outColor = mix(colTex1, colTex2, 0.5);
};
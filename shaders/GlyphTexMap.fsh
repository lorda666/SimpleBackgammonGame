#version 440 core

in vec2 textureCoords;
layout(binding = 1) uniform sampler2D texSampler;

out vec4 color;

void main(void)
{
	vec4 texColor = texture2D(texSampler, textureCoords);
	color = vec4(1.0f, 1.0f, 1.0f, texColor.r);
}

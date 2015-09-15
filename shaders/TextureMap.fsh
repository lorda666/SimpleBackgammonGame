#version 440 core

in vec2 textureCoords;
layout(binding = 0) uniform sampler2D texSampler;

out vec4 color;

void main(void)
{
	color = texture2D(texSampler, textureCoords);
}
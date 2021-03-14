//Code Modified from LearnOpenGL and from previous FLORP engine
#version 440

layout(location = 0) in vec2 inUV;
out vec4 fragColour;

layout(binding = 0) uniform sampler2D s_screenTex;
layout(binding = 1) uniform sampler2D s_bloomTex;

void main() 
{
	vec4 source = texture(s_screenTex, inUV);
	vec4 bloomSource = texture(s_bloomTex, inUV);

	fragColour = 1.0 - (1.0 - source) * (1.0 - bloomSource);
}
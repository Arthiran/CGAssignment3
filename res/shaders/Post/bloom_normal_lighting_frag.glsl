//Code Modified from LearnOpenGL and from previous FLORP engine
#version 440

layout(location = 0) in vec2 inUV;
out vec4 fragColour;

layout (binding = 0) uniform sampler2D s_screenTex;

void main() 
{
	vec4 source = texture(s_screenTex, inUV);

	fragColour.rgb = source.rgb; 
	fragColour.a = source.a; 
}
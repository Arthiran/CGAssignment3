//Code Modified from LearnOpenGL and from previous FLORP engine
#version 440

layout(location = 0) in vec2 inUV;
out vec4 fragColour;

layout(binding = 0) uniform sampler2D s_screenTex;

uniform float u_Threshold;
uniform float u_Intensity;

void main()
{
	vec4 source = texture(s_screenTex, inUV);
    
    float brightness = (source.r + source.g + source.b) / 3.0;
	if(brightness > u_Threshold)
	{
		fragColour = source;
	}
	else
	{
		fragColour = vec4(0.0, 0.0, 0.0, 1.0);
	}
}
//Code Modified from LearnOpenGL and from previous FLORP engine
#version 440

layout(location = 0) in vec2 inUV;
out vec4 fragColour;

layout(binding = 0) uniform sampler2D s_screenTex;

uniform bool u_Horizontal;
const float weights[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main() 
{
	vec4 source = texture(s_screenTex, inUV);
    vec2 off = 1.0 / textureSize(s_screenTex, 0);
	if (u_Horizontal) 
	{
		off = off * vec2(1, 0);
	} else 
	{
		off = off * vec2(0, 1);
	}
	vec4 result = source * weights[0];
	for(int i = 1; i < 5; i++) 
	{
		result += texture(s_screenTex, inUV + off * i) * weights[i];
		result += texture(s_screenTex, inUV - off * i) * weights[i];
	}
	fragColour = vec4(result.rgb, 1);
}
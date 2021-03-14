//Code Modified from shadertoy to work with OTTER: https://www.shadertoy.com/view/4sXSWs
#version 440

layout(location = 0) in vec2 inUV;

out vec4 fragColour;

layout (binding = 0) uniform sampler2D s_screenTex;

uniform float u_Time;
uniform float u_Strength = 16.0;

void main() 
{
	vec4 source = texture(s_screenTex, inUV);

	float x = (inUV.x + 4.0 ) * (inUV.y + 4.0 ) * (u_Time * 10.0);
	vec4 grain = vec4(mod((mod(x, 13.0) + 1.0) * (mod(x, 123.0) + 1.0), 0.01)-0.005) * u_Strength;
    
	fragColour = source + grain;
}
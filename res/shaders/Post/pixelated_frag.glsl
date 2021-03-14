// Original Pixelation shader: http://coding-experiments.blogspot.com/2010/06/pixelation.html
#version 440

layout (location = 0) in vec2 inUV;

layout (binding = 0) uniform sampler2D s_screenTex;

out vec4 fragColour;

uniform float u_Pixels = 512.0;

void main() 
{
    float dx = 5.0 * (1.0 / u_Pixels);
    float dy = 10.0 * (1.0 / u_Pixels);
	vec2 texCoords = vec2(dx * floor(inUV.x / dx), dy * floor(inUV.y / dy));
    fragColour = texture(s_screenTex, texCoords);
}
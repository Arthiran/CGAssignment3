#version 420

layout(location = 0) in vec2 inUV;

struct DirectionalLight
{
	//Light direction (defaults to down, to the left, and a little forward)
	vec4 _lightDirection;

	//Generic Light controls
	vec4 _lightCol;

	//Ambience controls
	vec4 _ambientCol;
	float _ambientPow;
	
	//Power controls
	float _lightAmbientPow;
	float _lightSpecularPow;
	
	float _shadowBias;
};

layout (std140, binding = 0) uniform u_Lights
{
	DirectionalLight sun;
};

layout (binding = 30) uniform sampler2D s_ShadowMap;

layout (binding = 0) uniform sampler2D s_albedoTex;
layout (binding = 1) uniform sampler2D s_normalsTex;
layout (binding = 2) uniform sampler2D s_specularTex;
layout (binding = 3) uniform sampler2D s_positionTex;

layout (binding = 4) uniform sampler2D s_lightAccumTex;

uniform mat4 u_LightSpaceMatrix;
uniform vec3 u_CamPos;

out vec4 frag_colour;

float ShadowCalculation(vec4 fragPosLightSpace, float bias)
{
	//Perspective division
	vec3 projectionCoordinates = fragPosLightSpace.xyz / fragPosLightSpace.w;
	
	//Transform into a [0,1] range
	projectionCoordinates = projectionCoordinates * 0.5 + 0.5;
	
	//Get the closest depth value from light's perspective (using our 0-1 range)
	float closestDepth = texture(s_ShadowMap, projectionCoordinates.xy).r;

	//Get the current depth according to the light
	float currentDepth = projectionCoordinates.z;

	//Check whether there's a shadow
	//float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

	//PCF
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(s_ShadowMap, 0);
	for(int i = -1; i <= 1; i++)
	{
	    for(int j = -1; j <= 1; j++)
	    {
	        float pcfDepth = texture(s_ShadowMap, projectionCoordinates.xy + vec2(i, j) * texelSize).r; 
	        shadow += currentDepth - sun._shadowBias > pcfDepth ? 1.0 : 0.0;        
	    }    
	}
	shadow /= 9.0;

	if (projectionCoordinates.z > 1.0)
	{
		shadow = 0.0;
	}

	//Return the value
	return shadow;
}

// https://learnopengl.com/Advanced-Lighting/Advanced-Lighting
void main() {
    //Albedo
    vec4 textureColour = texture(s_albedoTex, inUV);
    //Normals 
    vec3 inNormal = (normalize(texture(s_normalsTex, inUV).rgb) * 2.0) - 1.0;
    //Specular
    float texSpec = texture(s_specularTex, inUV).r;
    //Positions
    vec3 fragPos = texture(s_positionTex, inUV).rgb;

	// Diffuse
	vec3 N = normalize(inNormal);
	vec3 lightDir = normalize(-sun._lightDirection.xyz);
	float dif = max(dot(N, lightDir), 0.0);
    vec3 diffuse = sun._lightCol.xyz * dif; // add diffuse intensity

	// Specular
	vec3 viewDir  = normalize(u_CamPos - fragPos);
	vec3 h        = normalize(lightDir + viewDir);

	float spec = pow(max(dot(N, h), 0.0), 4.0); // Shininess coefficient (can be a uniform)
	vec3 specular = sun._lightSpecularPow * texSpec * spec * sun._lightCol.xyz; // Can also use a specular color

    vec4 fragPosLightSpace = u_LightSpaceMatrix * vec4(fragPos, 1.0);
	float shadow = ShadowCalculation(fragPosLightSpace, sun._shadowBias);

	vec3 result = (
		(sun._ambientPow * sun._ambientCol.xyz) + // global ambient light
		(1.0 - shadow) * //Shadow value
		(diffuse + specular)); // Object color

    if (textureColour.a < 0.31)
    {
        result = vec3(1.0, 1.0, 1.0);
    }

	frag_colour = vec4(result, 1.0);
}
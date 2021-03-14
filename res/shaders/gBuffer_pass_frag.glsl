#version 420

//Data for this model
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColour;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUV;

//The albedo textures
uniform sampler2D s_Diffuse;
uniform sampler2D s_Diffuse2;
uniform sampler2D s_Specular;
uniform float u_textureMix;

//MULTI RENDER TARGET
//We can render colour to all of these
layout(location = 0) out vec4 outColours;
layout(location = 1) out vec3 outNormals;
layout(location = 2) out vec3 outSpecs;
layout(location = 3) out vec3 outPositions;

void main()
{
    //Get the albedo from the diffuse / albedo map
    vec4 textureColour1 = texture(s_Diffuse, inUV);
    vec4 textureColour2 = texture(s_Diffuse2, inUV);
    vec4 textureColour = mix(textureColour1, textureColour2, u_textureMix);

    //Outputs texture colour
    outColours = textureColour;

    //Outputs normals colour
    //[-1, 1] -> inNormals range
    //[0, 1] -> OutNormals range
    //So we convert
    outNormals = (normalize(inNormal) * 0.5) + 0.5;

    //Outputs specular colours
    outSpecs = texture(s_Specular, inUV).rgb;

    //Outputs the viewspace positions
    outPositions = inPos;
}
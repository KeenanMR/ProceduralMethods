// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);
Texture2D texture2 : register(t2);
Texture2D texture3 : register(t3);
SamplerState sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
	float4 diffuseColour;
	float3 lightDirection;
	float padding;
};

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 worldPosition : POSITION;
};


// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
	float intensity = saturate(dot(normal, lightDirection));
	float4 colour = saturate(diffuse * intensity);
	return colour;
}

float4 main(InputType input) : SV_TARGET
{
	float4 textureColour;
	float4 lightColour;
	float4 tempTexture;

	float blendValue1, blendValue2, blendValue3;

	blendValue1 = input.worldPosition.y + 10 / -8 + 10;
	blendValue2 = input.worldPosition.y -4 / 20 - 4;

	float steepness = 1.0f - input.normal.y;


	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.

	if (input.worldPosition.y < -8)
	{
		textureColour = lerp(texture0.Sample(sampler0, input.tex), texture1.Sample(sampler0, input.tex), blendValue1);
	}
	if (input.worldPosition.y > -8 && input.worldPosition.y < 4)
	{
		tempTexture = texture1.Sample(sampler0, input.tex);
		if (steepness > 0.3f)
		{
			textureColour = lerp(tempTexture, texture3.Sample(sampler0, input.tex), steepness* 2);

		}
		else
			textureColour = tempTexture;
	}
	if (input.worldPosition.y > 4)
	{
		textureColour = lerp(texture1.Sample(sampler0, input.tex), texture2.Sample(sampler0, input.tex), blendValue2);
	
	}

	
	



	lightColour = calculateLighting(-lightDirection, input.normal, diffuseColour);
	
	return lightColour * textureColour;
}




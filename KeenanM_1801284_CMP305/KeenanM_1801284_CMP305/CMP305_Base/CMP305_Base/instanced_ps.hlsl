// Instance pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)


Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);
Texture2D texture2 : register(t2);
Texture2D texture3 : register(t3);
Texture2D texture4 : register(t4);
SamplerState sampler0 : register(s0);

cbuffer LightBuffer : register(b0) {
	float4 ambientColour;
	float4 diffuseColour;
	float3 lightDirection;
	float padding;
};

struct InputType {
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 worldPosition : POSITION;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse) {
	float intensity = saturate(dot(normal, lightDirection));
	float4 colour = saturate(diffuse * intensity);
	return colour;
}

float4 main(InputType input) : SV_TARGET
{
	float4 textureColour;
float4 lightColour;

// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
if (input.worldPosition.y < -6)
{
	textureColour = texture2.Sample(sampler0, input.tex);
}
if (input.worldPosition.y  >= -6 && input.worldPosition.y < -1)
{
	textureColour = texture0.Sample(sampler0, input.tex);
}
if (input.worldPosition.y < 6 && input.worldPosition.y >= -1)
{
	textureColour = texture1.Sample(sampler0, input.tex);

}
if (input.worldPosition.y >= 6 && input.worldPosition.y < 10)
{
	textureColour = texture3.Sample(sampler0, input.tex);
}
if (input.worldPosition.y >= 10)
{
	textureColour = texture4.Sample(sampler0, input.tex);

}

lightColour = ambientColour + calculateLighting(-lightDirection, input.normal, diffuseColour);

return lightColour * textureColour;
}

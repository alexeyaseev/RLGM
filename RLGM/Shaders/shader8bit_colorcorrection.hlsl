// Pixel shader input structure
struct PS_INPUT
{
	float4 Position   : SV_POSITION;
	float2 Texture    : TEXCOORD0;
};

// unified 8bit
Texture2D texture0 : register( t0 );
SamplerState texture0_Sampler : register( s0 );

// colorcorrection
Texture2D texture1 : register( t1 );
SamplerState texture1_Sampler : register( s1 );

float4 PS(PS_INPUT input) : SV_Target 
{
  float4 val8bit = texture0.Sample(texture0_Sampler, input.Texture);
  input.Texture[0] = val8bit[0]; //8bit ampl
  input.Texture[1] = val8bit[3]; //radar id
  return texture1.Sample(texture1_Sampler, input.Texture); 
}
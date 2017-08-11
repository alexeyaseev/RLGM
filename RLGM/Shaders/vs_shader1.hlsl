// Vertex shader input structure
struct VS_INPUT
{
    float4 Position   : POSITION;
    float2 Texture    : TEXCOORD0;
};

// Vertex shader output structure
struct VS_OUTPUT
{
    float4 Position   : SV_POSITION;
    float2 Texture    : TEXCOORD0;
};

cbuffer ConstantBuffer : register( b0 )
{
	matrix WorldViewProj;
}

VS_OUTPUT VS( in VS_INPUT In )
{
	VS_OUTPUT Out;

	Out.Position = mul(WorldViewProj, In.Position);
    Out.Texture  = In.Texture;

    return Out;
}
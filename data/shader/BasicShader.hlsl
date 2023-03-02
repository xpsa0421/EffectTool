//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D		g_texture	: register (t0);
SamplerState	g_samLinear	: register (s0);

cbuffer cb : register (b0)
{
	float4x4	g_matWorld	: packoffset(c0);
	float4x4	g_matView	: packoffset(c4);
	float4x4	g_matProj	: packoffset(c8);
	float4		g_meshColor : packoffset(c12);
};
//--------------------------------------------------------------------------------------

struct VertexIn
{
	float4 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
};

struct VertexOut
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VSMain(VS_INPUT vIn)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.p = mul(vIn.p, g_matWorld);
	output.p = mul(output.p, g_matView);
	output.p = mul(output.p, g_matProj);
	output.n = vIn.n;
	output.t = vIn.t;
	output.c = vIn.c * g_meshColor;
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PSMain(VS_OUTPUT vIn) : SV_Target
{
	return g_texture.Sample(g_samLinear, vIn.t) * vIn.c;
}

struct VertexIn
{
	float3 pos 		: POSITION;
	float4 color 	: COLOR;
	float2 tex 		: TEXTURE;
};

struct VertexOut
{
	float4 	center 	: SV_POSITION;
	float4 	color 	: COLOR0;
	float2 	tex 	: TEXCOORD0;
};

cbuffer cbPerObject : register(b0)
{
	matrix 	g_worldviewproj	: packoffset(c0);
};


VertexOut main(VertexIn v_in)
{
	VertexOut v_out;

	v_out.center	= v_in.pos;
	v_out.color		= v_in.color;
	v_out.tex		= v_in.tex;

	return v_out;
}
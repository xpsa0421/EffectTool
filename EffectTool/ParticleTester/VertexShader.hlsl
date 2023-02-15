struct VertexIn
{
	float3	pos 	: POSITION;
	float4	color 	: COLOR;
	float2	size	: SIZE;
	float3	tex 	: TEXCOORD;
	uint	tex_idx	: TEXIDX;
};

struct VertexOut
{
	float3 	center 	: POSITION;
	float4 	color 	: COLOR0;
	float2	size	: SIZE;
	float2 	tex 	: TEXCOORD0;
	uint	tex_idx	: TEXIDX;
};

VertexOut main(VertexIn v_in)
{
	VertexOut v_out;

	v_out.center	= v_in.pos;
	v_out.color		= v_in.color;
	v_out.size		= v_in.size;
	v_out.tex		= v_in.tex;
	v_out.tex_idx	= v_in.tex_idx;

	return v_out;
}
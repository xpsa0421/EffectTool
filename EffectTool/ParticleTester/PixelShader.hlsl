struct PixelIn
{
	float4	pos		: SV_POSITION;
	float4	color	: COLOR0;
	float2	tex		: TEXCOORD0;
	uint	tex_idx	: TEXIDX;
};

struct PixelOut
{
	float4 a : SV_TARGET0; // +
	float4 b : SV_TARGET1; // *
};

Texture2DArray  g_textures		: register(t0);
SamplerState  	g_sampleWrap	: register(s0);


PixelOut main(PixelIn p_in) : SV_Target
{
	PixelOut p_out;
	float3 uvw = float3(p_in.tex, p_in.tex_idx);
	float4 vColor = g_textures.Sample(g_sampleWrap, uvw);
	float alpha = max(max(vColor.r, vColor.g), vColor.b);
	p_out.b = float4(1.0f - alpha,
					1.0f - alpha,
					1.0f - alpha, 1.0f);
	p_out.a = vColor * p_in.color;
	return p_out;
}
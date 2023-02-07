cbuffer cbPerObject
{
	float4x4 world_view_proj;
};

struct VertexIn
{
	float3 pos		:	POSITION;
	float4 color	:	COLOR;
};

struct VertexOut
{
	float4	pos		:	SV_POSITION;
	float4	color	:	COLOR;
};

// Vertex shader
VertexOut VS(VertexIn v_in)
{
	VertexOut v_out;
	v_out.pos = mul(float4(v_in.pos, 1.0f), world_view_proj);
	v_out.color = v_in.color;
	return vOut;
}

// Pixel shader
float4 PS(VertexOut p_in) : SV_Target
{
	return p_in.color;
}

// technique
technique11 Color
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};
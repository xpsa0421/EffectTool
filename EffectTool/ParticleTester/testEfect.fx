cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
};

struct VertexIn
{
	float3 Pos		:	POSITION;
	float4 Color	:	COLOR;
};

struct VertexOut
{
	float4	PosH	:	SV_POSITION;
	float4	Color	:	COLOR;
};

// Vertex shader
VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.PosH = mul(float4(vIn.Pos, 1.0f), gWorldViewProj);
	vOut.Color = vIn.Color;
	return vOut;
}

// Pixel shader
float4 PS(VertexOut pIn) : SV_Target
{
	return pIn.Color;
}

// technique
technique11 ColorTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	};
};
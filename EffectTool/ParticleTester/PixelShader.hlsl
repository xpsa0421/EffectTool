struct PixelIn
{
	float4	pos		: SV_POSITION;
	float2	tex		: TEXCOORD0;
};

//Texture2D     	g_texture		: register(t0);
//SamplerState  	g_sampleWrap	: register(s0);


float4 main(PixelIn p_in) : SV_Target
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
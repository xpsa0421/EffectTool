cbuffer cbPerFrame : register(b0)
{
	float4x4	billboard;
	float4x4	view;
	float4x4	proj;
};

cbuffer cbPerObject : register(b1)
{
	int		axis_lock_type		;//: packoffset(c0.x);
	float	is_alpha_tested		;//: packoffset(c0.y);
	float	is_alpha_blended	;//: packoffset(c0.z);
	float	is_uv_animated		;//: packoffset(c0.w);
	float2	sprite_dimension	;//: packoffset(c1.x);
	float2	temp				;//: packoffset(c1.z);
};

struct GeoIn
{
	float3 	center 		: POSITION;
	float4 	color 		: COLOR0;
	float2	size		: SIZE;
	float2 	tex 		: TEXCOORD0; // contains row, col idx in uv-animation
	uint	tex_idx		: TEXIDX;	// unused in uv, contains texture idx in texture-animation
};

struct GeoOut
{
	float4	pos		: SV_POSITION;
	float4	color	: COLOR0;
	float2	tex		: TEXCOORD0;
	uint	tex_idx	: TEXIDX;
};

// expand each point into a quad
[maxvertexcount(4)]
void main(
	point GeoIn g_in[1],
	inout TriangleStream<GeoOut> tri_stream)
{
	const float2 g_tex_coord[4] =
	{
		float2(0.0, 0.0),
		float2(1.0, 0.0),
		float2(0.0, 1.0),
		float2(1.0, 1.0),
	};

	float half_width = 0.5f * g_in[0].size.x;
	float half_height = 0.5f * g_in[0].size.y;
	
	float3 right = { 1,0,0 };
	float3 up = { 0,1,0 };

	float4 vertices[4];
	vertices[0] = float4(g_in[0].center - half_width * right + half_height * up, 1.0f);
	vertices[1] = float4(g_in[0].center + half_width * right + half_height * up, 1.0f);
	vertices[2] = float4(g_in[0].center - half_width * right - half_height * up, 1.0f);
	vertices[3] = float4(g_in[0].center + half_width * right - half_height * up, 1.0f);

	// do something according to lock type
	float4x4 world_view_proj = mul(billboard, view);
	world_view_proj = mul(world_view_proj, proj);

	GeoOut g_out;
	[unroll(4)]
	for (int i = 0; i < 4; i++)
	{
		g_out.pos = mul(vertices[i], world_view_proj);
		g_out.color = g_in[0].color;
		g_out.tex = g_tex_coord[i];
		g_out.tex_idx = g_in[0].tex_idx;

		tri_stream.Append(g_out);
	}
	tri_stream.RestartStrip();
}
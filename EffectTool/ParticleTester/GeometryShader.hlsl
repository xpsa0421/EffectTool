cbuffer cbPerFrame : register(b0)
{
	float3 g_eye_pos;
};

cbuffer cbPerObject : register(b1)
{
	float4x4 g_view_proj;
};

struct GeoIn
{
	float3 	center 	: POSITION;
	float4 	color 	: COLOR0;
	float2 	tex 	: TEXCOORD0;
};

struct GeoOut
{
	float4	sv_pos	: SV_POSITION;
	float3	pos		: POSITION;
	//float3	normal	: NORMAL;
	float2	tex		: TEXCOORD;
};

float2 g_tex_coord[4] =
{
	float2(0.0f, 1.0f),
	float2(0.0f, 0.0f),
	float2(1.0f, 1.0f),
	float2(1.0f, 0.0f)
};


// expand each point into a quad
[maxvertexcount(4)]
void main(
	point GeoIn g_in[1],
	inout TriangleStream<GeoOut> tri_stream)
{
	// facing camera position
	float3 up = float3(0.0f, 1.0f, 0.0f);
	float3 look = g_eye_pos - g_in[0].center;
	look.y = 0;
	look = normalize(look);
	float3 right = cross(up, look);
	up = cross(look, right);
	up = normalize(up);
	right = normalize(right);

	// compute quad vertices in world space
	float2 size = { 2, 2 };
	float half_width = 0.5f * size.x;
	float half_height = 0.5f * size.y;

	float4 vertices[4];
	vertices[0] = float4(g_in[0].center + half_width * right - half_height * up, 1.0f);
	vertices[1] = float4(g_in[0].center + half_width * right + half_height * up, 1.0f);
	vertices[2] = float4(g_in[0].center - half_width * right - half_height * up, 1.0f);
	vertices[3] = float4(g_in[0].center - half_width * right + half_height * up, 1.0f);

	GeoOut g_out;
	[unroll(4)]
	for (int i = 0; i < 4; i++)
	{
		g_out.sv_pos = mul(vertices[i], g_view_proj);
		g_out.pos = vertices[i].xyz;
		//g_out.normal = look;
		g_out.tex = g_tex_coord[i];

		tri_stream.Append(g_out);
	}
}
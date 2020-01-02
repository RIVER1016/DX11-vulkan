cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gView;
	float4x4 gProjection;
};

cbuffer cbPerFrame
{
    float3 gLightDirection;
    float3 gLightPosition;
    float4 gLightColor;
};

struct VertexIn
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float4 Color : COLOR;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float4 Normal : NORMAL;
	float4 Color : COLOR;
};

VertexOut VS( VertexIn vin )
{
	VertexOut vout;
	vout.PosH = mul( float4( vin.Pos, 1.f ), gWorld );
	vout.PosH = mul( vout.PosH, gView );
	vout.PosH = mul( vout.PosH, gProjection );
	vout.Color = vin.Color;
	vout.Normal =  mul( float4( vin.Normal, 1.f ), gWorld );
	return vout;
};

float4 PS( VertexOut pin ) : SV_Target
{
	float3 l = -gLightDirection;
	float4 Color = pin.Color * dot( ( float3 )pin.Normal, l );
	return Color;
};

technique11 ColorTech
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VS( ) ) );
		SetPixelShader( CompileShader( ps_5_0, PS( ) ) );
	}
}


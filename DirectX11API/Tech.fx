cbuffer VSConstBuffer
{
	float4 gWorld[4];
	float4 gView[4];
	float4 gProjection[4];
};

cbuffer PSConstBuffer
{
	float4 gLightDirection;
	float4 gLightPosition;
	float4 gLightColor;
	float4 gEyePos;
	float4 materialSpecColor;
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
	float4 PosW : POSITION;
	float4 Normal : NORMAL;
	float4 Color : COLOR;
};

VertexOut VS( VertexIn vin )
{
	VertexOut vout;
	float4x4 World = float4x4( gWorld[0], gWorld[1], gWorld[2], gWorld[3] );
	float4x4 View = float4x4( gView[0], gView[1], gView[2], gView[3] );
	float4x4 Projection = float4x4( gProjection[0], gProjection[1], gProjection[2], gProjection[3] );
	vout.PosH = mul( float4( vin.Pos, 1.f ), World );
	vout.PosH = mul( vout.PosH, View );
	vout.PosH = mul( vout.PosH, Projection );
	vout.PosW = mul( float4( vin.Pos, 1.f ), World );
	vout.Color = vin.Color;
	vout.Normal =  mul( float4( vin.Normal, 1.f ), World );
	return vout;
};

float4 PS( VertexOut pin ) : SV_Target
{
	float3 l = -( float3 )gLightDirection;
	float3 n = ( float3 )pin.Normal;
	float nDotL = dot( ( float3 )pin.Normal, l );
	float3 r = reflect( l, n );
	float3 view = normalize( ( float3 )( gEyePos - pin.PosW ) );
	float3 spec = ( float3 )materialSpecColor * pow( saturate( dot( view, r ) ), 10.f );
	float4 Color = float4( ( float3 )pin.Color * nDotL + spec, 1.f );
	return Color;
};

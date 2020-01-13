cbuffer VSConstBuffer
{
	float4x4 gWorld;
	float4x4 gView;
	float4x4 gProjection;
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
	vout.PosH = mul( float4( vin.Pos, 1.f ), gWorld );
	vout.PosH = mul( vout.PosH, gView );
	vout.PosH = mul( vout.PosH, gProjection );
	vout.PosW = mul( float4( vin.Pos, 1.f ), gWorld );
	vout.Color = vin.Color;
	vout.Normal =  mul( float4( vin.Normal, 1.f ), gWorld );
	return vout;
};

float4 PS( VertexOut pin ) : SV_Target
{
	float3 l = - ( float3 )gLightDirection;
	float3 n = ( float3 )pin.Normal;
	float nDotL = dot( ( float3 )pin.Normal, l );
	float3 r = reflect( l, n );
	float3 view = normalize( ( float3 )( gEyePos - pin.PosW ) );
	float3 spec = ( float3 )materialSpecColor * pow( saturate( dot( view, r ) ), 10.f );
	float4 Color = float4( ( float3 )pin.Color * nDotL + spec, 1.f );
	return Color;
};

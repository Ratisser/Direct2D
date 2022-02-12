// #include "CbufferHeader.fx"
// b32 까지 가능했던걸로 아는데
// 그냥 생각하지 맙시다.

// 512바이트 가 최대일겁니다.


cbuffer TransformData : register(b0)
{
    float4x4 World;
    float4x4 View;
    float4x4 Projection;
};

float4 Texture_VS(float4 pos : POSITION) : SV_POSITION
{
    // pos *= World;
    pos = mul(pos, World);

    return pos;
}

float4 Texture_PS(float4 pos : SV_POSITION) : SV_Target0
{
    pos = mul(pos, World);

	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}




struct VSInput {
    float3 inPosition;
    float3 inNormal;
    float2 inUV;
};

struct UniformBuffer {
    float4x4 view;
    float4x4 proj;
};
ConstantBuffer<UniformBuffer> ubo;

struct PushConst {
    float4x4 model;
};
[[vk::push_constant]] PushConst pc;


struct VSOutput
{
    float4 pos : SV_Position;
    float3 normal;
    float3 color;
};

[shader("vertex")]
VSOutput vertMain(VSInput input) {
    VSOutput output;
    output.pos = mul(ubo.proj, mul(ubo.view, mul(pc.model, float4(input.inPosition, 1.0))));
    output.normal = mul((float3x3)pc.model, input.inNormal);
    output.normal = normalize(output.normal);
    output.color = float3 (1.0f, 1.0f, 1.0f);
    return output;
}

[shader("fragment")]
float4 fragMain(VSOutput vertIn) : SV_TARGET {
    return float4(vertIn.color, 1.0);
}

struct VSInput {
    float3 inPosition;
    float3 inNormal;
    float2 inUV;
};

struct Light
{
    float3 posWorld;
    float3 color;
    float intensity;
};

struct UniformBuffer
{
    float4x4 view;
    float4x4 proj;
    Light light;
};
ConstantBuffer<UniformBuffer> ubo;

struct PushConst {
    float4x4 model;
};
[[vk::push_constant]] PushConst pc;


struct VSOutput
{
    float4 pos : SV_Position;
    float3 posWorld;
    float3 normal;
    float3 color;
};

[shader("vertex")]
VSOutput vertMain(VSInput input) {
    VSOutput output;
    output.pos = mul(ubo.proj, mul(ubo.view, mul(pc.model, float4(input.inPosition, 1.0))));
    output.normal = mul((float3x3)pc.model, input.inNormal);
    output.posWorld = (mul(pc.model,float4(input.inPosition,1.0))).xyz;
    output.normal = normalize(output.normal);
    output.color = float3 (1.0f, 1.0f, 1.0f);
    return output;
}

[shader("fragment")]
float4 fragMain(VSOutput vertIn) : SV_TARGET
{
    Light light = ubo.light;
    float3 L = normalize(light.posWorld - vertIn.posWorld);
    float diffuse = max(0,dot(vertIn.normal,L));
    float distance = length(light.posWorld - vertIn.posWorld);
    float attenuation = 1 / (1 + 0.1f * pow(distance,2));
    float ambiant = 0.01f;
    vertIn.color = light.color * (ambiant + diffuse * attenuation * light.intensity);
    return float4(vertIn.color, 1.0);
}

struct PS_INPUT
{
    float4 Pos : SV_POSITION; // ���݂̃s�N�Z���ʒu
    float4 PosW : POSITION0;  // �I�u�W�F�N�g�̃��[���h���W
    float4 NorW : NORMAL0;  // �@��
};

// ���C�g
struct Light
{
    float4 pos;       // ���W
    float4 diffuse;   // �g�U
    float4 specular;  // ����
    float4 attenuate; // ����
};

// �}�e���A��
struct Material
{
    float4 ambient;  // ������
    float4 diffuse;  // �g�U����
    float4 specular; // ���ʔ���
};

cbuffer ConstantBuffer : register(b0)
{
    float4   eyePos;
    float4   ambient;
    Light    pointLight;
    Material material;
};

float4 PS(PS_INPUT input) : SV_TARGET
{
    float3 n;
    float3 v;
    float3 l;
    float3 r;
    float  d;
    float  a;
    float3 iA;
    float3 iD;
    float3 iS;

    // -- �t�H�����˃��f�� --
    n = normalize(input.NorW.xyz);
    v = normalize(eyePos.xyz - input.PosW.xyz);
    l = pointLight.pos.xyz - input.PosW.xyz;
    d = length(l);
    l = normalize(l);
    r = 2.0 * n * dot(n, l) - l;
    a = saturate(1.0 / (pointLight.attenuate.x + pointLight.attenuate.y * d + pointLight.attenuate.z * d * d));

    iA = material.ambient.xyz * ambient.xyz;
    iD = saturate(dot(l, n)) * material.diffuse.xyz * pointLight.diffuse.xyz * a;
    iS = pow(saturate(dot(r, v)), material.specular.w) * material.specular.xyz * pointLight.specular.xyz * a;

    return float4(saturate(iA + iD + iS), 1.0);
}
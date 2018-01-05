cbuffer ConstantBuffer : register(b0)
{
    matrix World;           // ���[���h�s��
    matrix View;            // �r���[�s��
    matrix Projection;      // �ˉe�s��
    float4 lightAmbient;    // ����
    float4 materialAmbient; // ���̂̐F
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION; // ���݂̃s�N�Z���ʒu
    float4 PosW : POSITION0;  // �I�u�W�F�N�g�̃��[���h���W
    float4 NorW : TEXCOORD0;  // �@��
};

float4 PS(PS_INPUT input) : SV_TARGET
{
    float3 i;

    i = materialAmbient.xyz * lightAmbient.xyz;

    return float4(i, 1.0);
}
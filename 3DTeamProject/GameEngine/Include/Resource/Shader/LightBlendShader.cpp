#include "LightBlendShader.h"

CLightBlendShader::CLightBlendShader()
{
}

CLightBlendShader::~CLightBlendShader()
{
}

bool CLightBlendShader::Init()
{
	if (!LoadVertexShader("LightAccVS", TEXT("Light.fx"), SHADER_PATH))
		return false;

	if (!LoadPixelShader("LightBlendPS", TEXT("Light.fx"), SHADER_PATH))
		return false;

	// ���������� Null Buffer �� �̿��ϱ� ������
	// ������ �Է� �����⸦ �Ѱ��� �ʿ䰡 ����.
	

	return true;
}

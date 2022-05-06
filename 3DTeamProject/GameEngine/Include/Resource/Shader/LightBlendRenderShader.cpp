#include "LightBlendRenderShader.h"

CLightBlendRenderShader::CLightBlendRenderShader()
{
}

CLightBlendRenderShader::~CLightBlendRenderShader()
{
}

bool CLightBlendRenderShader::Init()
{
	if (!LoadVertexShader("LightAccVS", TEXT("Light.fx"), SHADER_PATH))
		return false;

	if (!LoadPixelShader("LightBlendRenderPS", TEXT("Light.fx"), SHADER_PATH))
		return false;

	// ���������� Null Buffer �� �̿��ϱ� ������
	// ������ �Է� �����⸦ �Ѱ��� �ʿ䰡 ����.
	return true;
}

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

	// 마찬가지로 Null Buffer 를 이용하기 때문에
	// 별도로 입력 조립기를 넘겨줄 필요가 없다.
	return true;
}

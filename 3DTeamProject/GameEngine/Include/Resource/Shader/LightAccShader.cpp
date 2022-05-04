#include "LightAccShader.h"

CLightAccShader::CLightAccShader()
{
}

CLightAccShader::~CLightAccShader() //
{
}

bool CLightAccShader::Init()
{
	if (!LoadVertexShader("LightAccVS", TEXT("Light.fx"), SHADER_PATH))
		return false;

	if (!LoadPixelShader("LightAccPS", TEXT("Light.fx"), SHADER_PATH))
		return false;

	// Null Buffer 이다 -> 애초에 정점을 입력하지 않는다.
	// 따라서 입력 조립기를 세팅할 필요가 없다.
	
	return true;
}

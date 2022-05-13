#include "SkyObject.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CSkyObject::CSkyObject()
{
	SetTypeID<CSkyObject>();
}

CSkyObject::CSkyObject(const CSkyObject& obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
}

CSkyObject::~CSkyObject()
{
}

bool CSkyObject::Init()
{
	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");

	// Sky Mesh ¼¼ÆÃ
	m_Mesh->SetMesh("SpherePos");

	CMaterial* Material = m_Scene->GetResource()->FindMaterial("SkyMaterial");

	Material->SetRenderState("FrontFaceCulling");
	Material->SetRenderState("SkyDepth");

	// Sky Shader + Sky Texture
	m_Mesh->AddMaterial(Material);

	m_Mesh->SetLayerName("Back");
	
	return true;
}

void CSkyObject::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CSkyObject::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CSkyObject* CSkyObject::Clone()
{
	return new CSkyObject(*this);
}

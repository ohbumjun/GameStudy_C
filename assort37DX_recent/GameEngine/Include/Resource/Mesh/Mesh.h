#pragma once

#include "../../Ref.h"
#include "../Material/Material.h"

class CMesh :
	public CRef
{
	friend class CMeshManager;

protected:
	CMesh();
	virtual ~CMesh() = 0;

protected:
	class CScene* m_Scene;

public:
	void SetScene(class CScene* Scene)
	{
		m_Scene = Scene;
	}

protected:
	std::vector<MeshContainer*>	m_vecContainer;
	// FBX 상에서 Mesh 를 Load 할 때, Material 까지 같이 Load 하고 있으므로
	// 재질 정보를 담아두기 위해 추가 Vector 도 만들어준다.
	std::vector<MeshSlot*>		m_vecMeshSlot;

	// Mesh 가 왜 여러개의 Material 을 들고 있는 것일까 ?
	// 왜냐하면, 같은 Mesh 라고 하더라도, Mesh 의 각 Subset 별로
	// 보여지는 질감이 다르기 때문이다.
	std::vector<CSharedPtr<CMaterial>>	m_vecMaterialSlot;

	Vector3	m_Min;
	Vector3	m_Max;
	Mesh_Type	m_MeshType;

public:
	const std::vector<CSharedPtr<CMaterial>>* GetMaterialSlots()	const
	{
		return &m_vecMaterialSlot;
	}

	void AddMaterialSlot(CMaterial* Material)
	{
		m_vecMaterialSlot.push_back(Material);
	}

	void AddMaterialSlot(const std::string& Name);

public:
	virtual bool CreateMesh(void* VtxData, int Size, int Count, D3D11_USAGE Usage,
		D3D11_PRIMITIVE_TOPOLOGY Primitive, void* IdxData = nullptr, int IdxSize = 0, int IdxCount = 0,
		D3D11_USAGE IdxUsage = D3D11_USAGE_DEFAULT, DXGI_FORMAT Fmt = DXGI_FORMAT_UNKNOWN);
	bool LoadMesh(const TCHAR* FileName, const std::string& PathName = MESH_PATH);
	bool LoadMeshFullPath(const TCHAR* FullPath);
	bool LoadMeshMultibyte(const char* FileName, const std::string& PathName = MESH_PATH);
	virtual bool LoadMeshFullPathMultibyte(const char* FullPath);
	virtual bool Init();
	virtual void Render();
	virtual void Render(int SlotNumber);
	virtual void RenderInstancing(int Count);
	virtual void RenderInstancing(ID3D11Buffer* InstancingBuffer, unsigned int InstanceSize, int Count);

protected:
	bool CreateBuffer(Buffer_Type Type, void* Data, int Size, int Count,
		D3D11_USAGE Usage, ID3D11Buffer** Buffer);


public:
	bool SaveMeshFile(const char* FullPath);
	bool LoadMeshFile(const char* FullPath);
	virtual bool ConvertFBX(class CFBXLoader* Loader, const char* FullPath);
	virtual bool SaveMesh(FILE* File);
	virtual bool LoadMesh(FILE* File);
};


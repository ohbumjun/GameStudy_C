#pragma once
#include "Mesh.h"
class CAnimationMesh :
    public CMesh
{
	friend class CMeshManager;

protected:
	CAnimationMesh();
	virtual ~CAnimationMesh();

private:
	CSharedPtr<class CSkeleton>	m_Skeleton;

public:
	class CSkeleton* GetSkeleton()	const
	{
		return m_Skeleton;
	}

public:
	virtual bool Init();
	void SetSkeleton(class CSkeleton* Skeleton);
	void SetSkeleton(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = MESH_PATH);
	virtual bool LoadMeshFullPathMultibyte(const char* FullPath);


public:
	virtual bool ConvertFBX(class CFBXLoader* Loader, const char* FullPath);
	virtual bool SaveMesh(FILE* File);
	virtual bool LoadMesh(FILE* File);
};


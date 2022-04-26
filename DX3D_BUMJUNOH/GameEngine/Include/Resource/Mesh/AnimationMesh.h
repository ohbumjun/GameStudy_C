#pragma once
#include "Mesh.h"
class CAnimationMesh :
    public CMesh
{
	friend class CMeshManager;

protected:
	CAnimationMesh();
	virtual ~CAnimationMesh();

	// Skeleton 을 들고 있는 녀석이 AnimationMesh 이다.
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
	// 여기서 Mesh 저보를 모두 뽑아오는데, 이 과정에서 Bone 정보도 뽑아올 것이다.
	virtual bool ConvertFBX(class CFBXLoader* Loader, const char* FullPath);
	virtual bool SaveMesh(FILE* File);
	virtual bool LoadMesh(FILE* File);
};

/*
기본적으로 Bone Animation 중심으로 돌아가게 할 것이다.

- 각 Mesh 안쪽에 Mesh 에 영향을 주는 Bone 이 있다.

- 즉, Animation 을 돌리는 것은, Bone 을 이동시키고 , 회전시키는 것이다.

- 그렇다면, Bone 정보가 필요하다.

- Bone 을 움직이는 방식은, Key Frame Animation 을 통해 움직이게 한다.
  1 Frame 당 모든 Bone 이 각각 얼만큼 이동시키는지.를 기준으로 세팅한다는 것이다.
  
- 즉, 2D 처럼 Animation Texture 를 여러 개 들고 있는 것이 아니라, Bone 처럼 이동 시키는 것이다.
 */
#pragma once

#include "../../Ref.h"

struct Bone
{
	std::string		strName;
	int				iDepth;
	int				iParentIndex;
	Matrix		matOffset; // 해당 Bone Space 로의 변환 ...?
	Matrix		matBone;
	int				iRefCount;

	Bone() :
		iRefCount(1)
	{
	}
};

class CSkeleton :
    public CRef
{
	friend class CAnimationManager3D;
	friend class CAnimationMesh;
	friend class CAnimationSequence;
	friend class CAnimationSequenceInstance;

private:
	CSkeleton();
	CSkeleton(const CSkeleton& skeleton);
	~CSkeleton();

private:
	std::vector<Bone*>				m_vecBones;
	// 맨 처음, 각 Bone Space 로의 변환 정보를 담은 구조화 버퍼 -> 처음 딱 한번만 계산 -> 딱 한번만 UpdateBuffer 호출
	class CStructuredBuffer* m_pOffsetMatrixBuffer;
	class CScene* m_Scene;
	//std::vector<class CBoneSocket*>	m_BoneSocket;

public:
	size_t GetBoneCount()	const;
	Bone* GetBone(int iIndex = 0)	const;
	Bone* GetBone(const std::string& strName)	const;
	int GetBoneIndex(const std::string& strName)	const;
	bool CheckBone(const std::string& Name)	const;
	const Matrix& GetBoneMatrix(const std::string& strName)	const;
	const Matrix& GetBoneMatrix(int iIndex)	const;
	//class CBoneSocket* GetSocket(const std::string& Name);

public:
	void AddBone(Bone* pBone);
	bool SaveSkeleton(const char* pFileName, const std::string& strPathName = MESH_PATH);
	bool SaveSkeletonFullPath(const char* pFullPath);
	bool LoadSkeleton(class CScene* pScene, const std::string& strName, const char* pFileName, const std::string& strPathName = MESH_PATH);
	bool LoadSkeletonFullPath(class CScene* pScene, const std::string& strName, const char* pFullPath);
	void SetShader();
	void ResetShader();
	//void AddSocket(const std::string& BoneName, const std::string& SocketName,
	//	const Vector3& Offset = Vector3::Zero, const Vector3& OffsetRot = Vector3::Zero);
	void Update(float fTime, const std::vector<Matrix>& vecBoneMatrix, const Matrix& matWorld);
	CSkeleton* Clone();
};


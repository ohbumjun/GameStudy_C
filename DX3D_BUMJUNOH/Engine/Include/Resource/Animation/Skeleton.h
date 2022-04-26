#pragma once

#include "../../Ref.h"

struct Bone
{
	std::string		strName; // 이름 
	int				iDepth; // 트리에서의 깊이
	int				iParentIndex; // 부모 Bone Idx
	Matrix		matOffset;
	Matrix		matBone; // 해당 Bone Space 로의 변환 ...?
	int				iRefCount;

	Bone() :
		iRefCount(1)
	{
	}
};

// Skeleton --> Bone 들을 가지고 있게 세팅할 것이다.
class CSkeleton :
    public CRef
{
	friend class CAnimationManager;
	friend class CAnimationMesh;
	friend class CAnimationSequence;
	friend class CAnimation;

private:
	CSkeleton();
	CSkeleton(const CSkeleton& skeleton);
	~CSkeleton();

private:
	std::vector<Bone*>				m_vecBones;

	// 구조화 버퍼를 사용한다 -->각 Bone 에 대한 Transformation Matrix --> Animation Matrix 정보를 매 Frame 마다
	// 넘겨줄 것이다 --> Matrix Appedix 기법 ..?
	// Key Frame 을 이용해서, 보간 처리를 한 다음, Bone 정보를 뽑는 것을 계산 셰이더에서 뽑아낼 것이다.
	class CStructuredBuffer* m_pOffsetMatrixBuffer;

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
	// Bone 정보를 Shader 측에 넘겨주는 함수
	void SetShader();
	void ResetShader();
	//void AddSocket(const std::string& BoneName, const std::string& SocketName,
	//	const Vector3& Offset = Vector3::Zero, const Vector3& OffsetRot = Vector3::Zero);
	void Update(float fTime, const std::vector<Matrix>& vecBoneMatrix, const Matrix& matWorld);
	CSkeleton* Clone();
};


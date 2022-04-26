#pragma once

#include "../../Ref.h"

struct Bone
{
	std::string		strName; // �̸� 
	int				iDepth; // Ʈ�������� ����
	int				iParentIndex; // �θ� Bone Idx
	Matrix		matOffset;
	Matrix		matBone; // �ش� Bone Space ���� ��ȯ ...?
	int				iRefCount;

	Bone() :
		iRefCount(1)
	{
	}
};

// Skeleton --> Bone ���� ������ �ְ� ������ ���̴�.
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

	// ����ȭ ���۸� ����Ѵ� -->�� Bone �� ���� Transformation Matrix --> Animation Matrix ������ �� Frame ����
	// �Ѱ��� ���̴� --> Matrix Appedix ��� ..?
	// Key Frame �� �̿��ؼ�, ���� ó���� �� ����, Bone ������ �̴� ���� ��� ���̴����� �̾Ƴ� ���̴�.
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
	// Bone ������ Shader ���� �Ѱ��ִ� �Լ�
	void SetShader();
	void ResetShader();
	//void AddSocket(const std::string& BoneName, const std::string& SocketName,
	//	const Vector3& Offset = Vector3::Zero, const Vector3& OffsetRot = Vector3::Zero);
	void Update(float fTime, const std::vector<Matrix>& vecBoneMatrix, const Matrix& matWorld);
	CSkeleton* Clone();
};


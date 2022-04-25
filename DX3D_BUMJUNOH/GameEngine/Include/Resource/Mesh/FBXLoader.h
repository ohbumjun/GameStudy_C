#pragma once

#include "fbxsdk.h"
#include "../../GameInfo.h"

#ifdef _DEBUG

#pragma comment(lib, "libfbxsdk-md_Debug.lib")

#else

#pragma comment(lib, "libfbxsdk-md.lib")

#endif // _DEBUG

// Bone
typedef struct _tagFbxBone
{
	std::string	strName;
	int		iDepth;
	int		iParentIndex;
	FbxAMatrix	matOffset;
	FbxAMatrix	matBone;
}FBXBONE, * PFBXBONE;

// Key Frame
typedef	struct _tagFbxKeyFrame
{
	FbxAMatrix	matTransform;
	double		dTime;
}FBXKEYFRAME, * PFBXKEYFRAME;

// Bone Key Frame
typedef	struct _tagFbxBoneKeyFrame
{
	int		iBoneIndex;
	std::vector<FBXKEYFRAME>	vecKeyFrame;
}FBXBONEKEYFRAME, * PFBXBONEKEYFRAME;

// Fbx Animation Clip
typedef struct _tagFbxAnimationClip
{
	std::string			strName;
	FbxTime			tStart;
	FbxTime			tEnd;
	FbxLongLong		lTimeLength;
	FbxTime::EMode	eTimeMode;
	std::vector<FBXBONEKEYFRAME>	vecBoneKeyFrame;
}FBXANIMATIONCLIP, * PFBXANIMATIONCLIP;

typedef struct _tagFbxWeight
{
	int		iIndex;
	double	dWeight;
}FBXWEIGHT, * PFBXWEIGHT;

typedef struct _tagFbxMaterial
{
	Vector4			BaseColor; // Diffuse �� �����Ǵ� ������ �ǹ��Ѵ�.
	Vector4			Amb; // Ambient
	Vector4			Spc; // ���ݻ� Specular
	Vector4			Emv; // Emissive 
	float			SpecularPower; // ���ݻ� �ݻ� ���� ...?
	float			TransparencyFactor;
	float			Shininess; // ��ü ǥ���� ��������� ���� ? --> ���ݻ��� ������ ����...?
	std::string		DifTex;
	std::string		BumpTex;
	std::string		SpcTex;
}FBXMATERIAL, * PFBXMATERIAL;

typedef struct _tagFBXMeshContainer
{
	std::vector<Vector3>	vecPos; // Pos ����
	std::vector<Vector3>	vecNormal;  // Normal Vector ���� 
	std::vector<Vector2>	vecUV; // Texture Coordinate ����
	std::vector<Vector3>	vecTangent;		// ��������
	std::vector<Vector3>	vecBinormal;	    // ����������
	std::vector<Vector4>	vecBlendWeight;	// �ִϸ��̼� ���� ���� --> �ش� Mesh �� ������ �ִ� �� Bone �� ����� 
	std::vector<Vector4>	vecBlendIndex;	// �ִϸ��̼� ���� ���� --> �ش� Mesh�� ������ �ִ� �� Bone, �� Bone ���� ����Ű�� Index ������

	// Idx ���۸� �����ϴ� ��
	// ���� ���� ����. ���·� �Ǿ� �ִ� ������ --> �ϳ��� �����̳� �ȿ�, ���� ���� Subset �� ���� �� �ֱ� �����̴�.
	// Subset �̶�, Mesh�� Ư�� ���� ex) �� Mesh --> �ָ� �κ�, �̵�
	std::vector<std::vector<UINT>>	vecIndices; 

	std::unordered_map<int, std::vector<FBXWEIGHT>>	mapWeights;	// �ִϸ��̼� ���� ����
	bool				Bump;
	bool				Animation;

	_tagFBXMeshContainer()
	{
		Bump = false;
		Animation = false;
	}
}FBXMESHCONTAINER, * PFBXMESHCONTAINER;

class CFBXLoader
{
	friend class CMesh;
	friend class CStaticMesh;
	friend class CAnimationMesh;
	friend class CAnimationSequence;

private:
	CFBXLoader();
	~CFBXLoader();

private:
	FbxManager* m_Manager;

	// FBXScene �ȿ�, ��Ʈ ��带 �ϳ� ������ �ִ�.
	// �ش� ��Ʈ ���� �ڽ� ������ ������ �ִ�. ��, �⺻ Ʈ�������� �Ǿ� �ִ�.
	// ex) Mesh, Material, Animation ������ ������ ��尡 �д��ؼ� ������ ������ �ְ� �� ���̴�.
	FbxScene* m_Scene;

private:
	// Material �� ���� �ȿ� ���� --> ��, Subset ����, ���� ������ ���� �ٸ��� ���� ������, �̿� ���� ������ ���� ���̴�.
	std::vector<std::vector<PFBXMATERIAL>>	m_vecMaterials;
	std::vector<PFBXMESHCONTAINER>		m_vecMeshContainer;
	std::vector<PFBXBONE>				m_vecBones;
	FbxArray<FbxString*>				m_NameArr;
	std::vector<PFBXANIMATIONCLIP>		m_vecClip;
	bool								m_Mixamo;

public:
	const std::vector<std::vector<PFBXMATERIAL>>* GetMaterials()	const
	{
		return &m_vecMaterials;
	}

	const std::vector<PFBXMESHCONTAINER>* GetMeshContainers()	const
	{
		return &m_vecMeshContainer;
	}

	const std::vector<PFBXBONE>* GetBones()	const
	{
		return &m_vecBones;
	}

	const std::vector<PFBXANIMATIONCLIP>* GetClips()	const
	{
		return &m_vecClip;
	}

public:
	// Static �� True ���, Animation�� �ִ� �༮
	// False ���, Animation �� ���� �༮
	// ��, �ش� Mesh�� Animation�� �ִ��� �������� �����ؼ� ���� ���̴�.
	bool LoadFBX(const char* FullPath, bool Static = true);

private:
	void Triangulate(FbxNode* pNode);
	void LoadMaterial(FbxSurfaceMaterial* pMtrl);
	Vector4 GetMaterialColor(FbxSurfaceMaterial* pMtrl,
		const char* pPropertyName, const char* pPropertyFactorName);
	double GetMaterialFactor(FbxSurfaceMaterial* pMtrl,
		const char* pPropertyName);
	std::string GetMaterialTexture(FbxSurfaceMaterial* pMtrl, const char* pPropertyName);

	void LoadMesh(FbxNode* pNode, bool bStatic);
	void LoadMesh(FbxMesh* pMesh, bool bStatic);

	void LoadNormal(FbxMesh* pMesh, PFBXMESHCONTAINER pContainer, int iVtxID,
		int iControlIndex);
	void LoadUV(FbxMesh* pMesh, PFBXMESHCONTAINER pContainer,
		int iUVID, int iControlIndex);
	void LoadTangent(FbxMesh* pMesh,
		PFBXMESHCONTAINER pContainer, int iVtxID, int iControlIndex);
	void LoadBinormal(FbxMesh* pMesh, PFBXMESHCONTAINER pContainer,
		int iVtxID, int iControlIndex);

private:
	void LoadAnimationClip();
	void LoadBone(FbxNode* pNode);
	void LoadBoneRecursive(FbxNode* pNode, int iDepth, int iIndex, int iParent);
	void LoadAnimation(FbxMesh* pMesh, PFBXMESHCONTAINER pContainer);
	FbxAMatrix GetTransform(FbxNode* pNode);
	int FindBoneFromName(const std::string& strName);
	void LoadWeightAndIndex(FbxCluster* pCluster,
		int iBoneIndex, PFBXMESHCONTAINER pContainer);
	void LoadOffsetMatrix(FbxCluster* pCluster,
		const FbxAMatrix& matTransform, int iBoneIndex,
		PFBXMESHCONTAINER pContainer);
	void LoadTimeTransform(FbxNode* pNode,
		FbxCluster* pCluster, const FbxAMatrix& matTransform,
		int iBoneIndex);
	void ChangeWeightAndIndices(PFBXMESHCONTAINER pContainer);
};

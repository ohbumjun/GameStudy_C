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
	Vector4			BaseColor; // Diffuse 에 대응되는 색상을 의미한다.
	Vector4			Amb; // Ambient
	Vector4			Spc; // 정반사 Specular
	Vector4			Emv; // Emissive 
	float			SpecularPower; // 정반사 반사 강도 ...?
	float			TransparencyFactor;
	float			Shininess; // 물체 표면의 번들번들한 정도 ? --> 정반사의 강도를 결정...?
	std::string		DifTex;
	std::string		BumpTex;
	std::string		SpcTex;
}FBXMATERIAL, * PFBXMATERIAL;

typedef struct _tagFBXMeshContainer
{
	std::vector<Vector3>	vecPos; // Pos 정보
	std::vector<Vector3>	vecNormal;  // Normal Vector 정보 
	std::vector<Vector2>	vecUV; // Texture Coordinate 정보
	std::vector<Vector3>	vecTangent;		// 접선벡터
	std::vector<Vector3>	vecBinormal;	    // 종법선벡터
	std::vector<Vector4>	vecBlendWeight;	// 애니메이션 관련 정보 --> 해당 Mesh 에 영향을 주는 각 Bone 의 영향력 
	std::vector<Vector4>	vecBlendIndex;	// 애니메이션 관련 정보 --> 해당 Mesh에 영향을 주는 각 Bone, 그 Bone 들을 가리키는 Index 정보들

	// Idx 버퍼를 저장하는 것
	// 벡터 안의 벡터. 형태로 되어 있는 이유는 --> 하나의 컨테이너 안에, 여러 개의 Subset 을 가질 수 있기 때문이다.
	// Subset 이란, Mesh의 특정 부위 ex) 팔 Mesh --> 주먹 부분, 이두
	std::vector<std::vector<UINT>>	vecIndices; 

	std::unordered_map<int, std::vector<FBXWEIGHT>>	mapWeights;	// 애니메이션 관련 정보
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

	// FBXScene 안에, 루트 노드를 하나 가지고 있다.
	// 해당 루트 노드는 자식 노드들을 가지고 있다. 즉, 기본 트리구조로 되어 있다.
	// ex) Mesh, Material, Animation 정보를 각각의 노드가 분담해서 정보를 가지고 있게 될 것이다.
	FbxScene* m_Scene;

private:
	// Material 도 벡터 안에 벡터 --> 즉, Subset 마다, 재질 정보가 각각 다르게 들어가기 때문에, 이와 같은 구성을 취한 것이다.
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
	// Static 이 True 라면, Animation이 있는 녀석
	// False 라면, Animation 이 없는 녀석
	// 즉, 해당 Mesh가 Animation이 있는지 없는지를 구분해서 만들 것이다.
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

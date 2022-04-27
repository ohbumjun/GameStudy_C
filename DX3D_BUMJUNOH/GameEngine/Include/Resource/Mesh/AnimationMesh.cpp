
#include "AnimationMesh.h"
#include "FBXLoader.h"
#include "../Animation/Skeleton.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"

CAnimationMesh::CAnimationMesh()
{
	SetTypeID<CAnimationMesh>();
	m_MeshType = Mesh_Type::Animation;
}

CAnimationMesh::~CAnimationMesh()
{
}

// fbx
bool CAnimationMesh::Init()
{
	return true;
}

void CAnimationMesh::SetSkeleton(CSkeleton* Skeleton)
{
	m_Skeleton = Skeleton;
}

void CAnimationMesh::SetSkeleton(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	m_Skeleton = new CSkeleton;

	char	FileNameMultibyte[MAX_PATH] = {};

	int Length = WideCharToMultiByte(CP_ACP, 0, FileName, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, FileName, -1, FileNameMultibyte, Length, 0, 0);

	m_Skeleton->LoadSkeleton(m_Scene, Name, FileNameMultibyte, PathName);
}

bool CAnimationMesh::LoadMeshFullPathMultibyte(const char* FullPath)
{
	char    Ext[_MAX_EXT] = {};

	_splitpath_s(FullPath, 0, 0, 0, 0, 0, 0, Ext, _MAX_EXT);
	_strupr_s(Ext);

	// FBX 파일을, 우리의 File 형태로 바꿀 것이다.
	if (strcmp(Ext, ".FBX") == 0)
	{
		CFBXLoader  Loader;

		if (!Loader.LoadFBX(FullPath, false))
			return false;

		// 우리의 파일 형태로 바꿔준다.
		return ConvertFBX(&Loader, FullPath);
	}

	return LoadMeshFile(FullPath);
}

bool CAnimationMesh::ConvertFBX(CFBXLoader* Loader, const char* FullPath)
{
	if (!CMesh::ConvertFBX(Loader, FullPath))
		return false;

    // 애니메이션 처리
	const std::vector<PFBXBONE>* pvecBone = Loader->GetBones();

	if (pvecBone->empty())
		return true;

	// Mesh 이름에 이름을 만들고, Skeleton Class 하나를 만든다.
	std::string	strSkeletonName = m_Name + "_Skeleton";

	m_Skeleton = new CSkeleton;

	// 본 수만큼 반복한다.
	std::vector<PFBXBONE>::const_iterator	iterB;
	std::vector<PFBXBONE>::const_iterator	iterBEnd = pvecBone->end();

	for (iterB = pvecBone->begin(); iterB != iterBEnd; ++iterB)
	{
		// 전체 Bone 들을 반복하면서, Bone Class 를 만들어낼 것이다.
		Bone* pBone = new Bone;

		pBone->strName = (*iterB)->strName;
		pBone->iDepth = (*iterB)->iDepth;
		pBone->iParentIndex = (*iterB)->iParentIndex;

		// 일반적인 Bone 행렬이, 우리가 사용하는 Bone 공간과 다르다.
		// 따라서, Offset 행렬을 사용해서, 우리가 사용하는
		// Local Bone 공간으로 가져와야 한다.
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				// 4 * 4 행렬을 matData 안의 matData 형태로 표현하고 있다.
				pBone->matOffset[i][j] = (float)(*iterB)->matOffset.mData[i].mData[j];
			}
		}

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				pBone->matBone[i][j] = (float)(*iterB)->matBone.mData[i].mData[j];
			}
		}

		m_Skeleton->AddBone(pBone);
	}

	// 애니메이션 클립을 추가한다.
	const std::vector<PFBXANIMATIONCLIP>* pvecClip = Loader->GetClips();

	// 파일 이름을 제외한 경로를 만들어둔다.
	char	strAnimPath[MAX_PATH] = {};
	strcpy_s(strAnimPath, FullPath);

	int 	iLength = (int)strlen(strAnimPath);

	for (int i = iLength - 1; i >= 0; --i)
	{
		// aa/bb.exe 9개, 2번인덱스 3 ~ 8번까지 제거
		if (strAnimPath[i] == '/' || strAnimPath[i] == '\\')
		{
			memset(&strAnimPath[i + 1], 0, sizeof(TCHAR) * (iLength - (i + 1)));
			break;
		}
	}

	// 클립을 읽어온다.
	std::vector<PFBXANIMATIONCLIP>::const_iterator	iterC;
	std::vector<PFBXANIMATIONCLIP>::const_iterator	iterCEnd = pvecClip->end();

	for (iterC = pvecClip->begin(); iterC != iterCEnd; ++iterC)
	{
		// 각 Clip 하나가 하나의 Motion, 즉, 하나의 Clip 이다.
		m_Scene->GetResource()->LoadAnimationSequence((*iterC)->strName, false, *iterC);

		// 중복 방지
		CAnimationSequence* pSequence = m_Scene->GetResource()->FindAnimationSequence((*iterC)->strName);

		if (!pSequence)
			continue;

		// 각각의 Animation 파일들을 .sqc 형태로 저장할 것이다.
		// .bne 파일과 .sqc 파일의 경우, 저장하는 파일 이름을 동일하게 세팅할 것이다.
		char	strAnimFullPath[MAX_PATH] = {};
		strcpy_s(strAnimFullPath, FullPath);
		memcpy(&strAnimFullPath[iLength - 3], "bne", 3);

		pSequence->SaveFullPathMultibyte(strAnimFullPath);
	}

	// 한편 Skeleton이 있다면, .bne 형식으로 저장할 것이다.
	if (m_Skeleton)
	{
		char	SkeletonPath[MAX_PATH] = {};
		strcpy_s(SkeletonPath, FullPath);
		memcpy(&SkeletonPath[iLength - 3], "bne", 3);
		m_Skeleton->SaveSkeletonFullPath(SkeletonPath);
	}

	return true;
}

bool CAnimationMesh::SaveMesh(FILE* File)
{
	if (!CMesh::SaveMesh(File))
		return false;

	return true;
}

bool CAnimationMesh::LoadMesh(FILE* File)
{
	if (!CMesh::LoadMesh(File))
		return false;

	return true;
}

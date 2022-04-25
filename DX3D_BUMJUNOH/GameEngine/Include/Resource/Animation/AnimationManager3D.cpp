
#include "AnimationManager3D.h"
#include "../../PathManager.h"

CAnimationManager3D::CAnimationManager3D()
{
}

CAnimationManager3D::~CAnimationManager3D()
{
}

bool CAnimationManager3D::Init()
{
	return true;
}

bool CAnimationManager3D::LoadAnimationSequence(const std::string& Name, bool Loop, 
	_tagFbxAnimationClip* Clip, class CScene* Scene)
{
	if (FindAnimationSequence(Name))
	{
		return true;
	}

	CAnimationSequence* Sequence = new CAnimationSequence;

	Sequence->m_Scene = Scene;

	if (!Sequence->CreateSequence(Loop, Clip))
	{
		SAFE_DELETE(Sequence);
		return false;
	}

	Sequence->SetName(Name);

	m_mapSequence.insert(std::make_pair(Name, Sequence));

	return true;
}

bool CAnimationManager3D::LoadAnimationSequence(const std::string& Name, bool Loop, 
	int StartFrame, int EndFrame, float PlayTime,
	const std::vector<BoneKeyFrame*>& vecFrame, class CScene* Scene)
{
	if (FindAnimationSequence(Name))
	{
		return true;
	}

	CAnimationSequence* Sequence = new CAnimationSequence;

	Sequence->m_Scene = Scene;

	if (!Sequence->CreateSequence(Name, Loop, StartFrame, EndFrame, PlayTime, vecFrame))
	{
		SAFE_DELETE(Sequence);
		return false;
	}

	Sequence->SetName(Name);

	m_mapSequence.insert(std::make_pair(Name, Sequence));

	return true;
}

bool CAnimationManager3D::LoadAnimationSequence(bool Loop, const std::string& Name,
	const TCHAR* FileName, const std::string& PathName, class CScene* Scene)
{
	if (FindAnimationSequence(Name))
	{
		return true;
	}

	CAnimationSequence* Sequence = new CAnimationSequence;

	Sequence->m_Scene = Scene;

	if (!Sequence->CreateSequence(Loop, Name, FileName, PathName))
	{
		SAFE_DELETE(Sequence);
		return false;
	}

	Sequence->SetName(Name);

	m_mapSequence.insert(std::make_pair(Name, Sequence));

	return true;
}

bool CAnimationManager3D::LoadAnimationSequenceFullPath(bool Loop, const std::string& Name,
	const TCHAR* FullPath, class CScene* Scene)
{
	if (FindAnimationSequence(Name))
	{
		return true;
	}

	CAnimationSequence* Sequence = new CAnimationSequence;

	Sequence->m_Scene = Scene;

	if (!Sequence->CreateSequence(FullPath))
	{
		SAFE_DELETE(Sequence);
		return false;
	}

	Sequence->m_Loop = Loop;
	Sequence->SetName(Name);

	m_mapSequence.insert(std::make_pair(Name, Sequence));

	return true;
}

bool CAnimationManager3D::LoadAnimationSequenceMultibyte(bool Loop, const std::string& Name,
	const char* FileName, const std::string& PathName, class CScene* Scene)
{
	if (FindAnimationSequence(Name))
	{
		return true;
	}

	CAnimationSequence* Sequence = new CAnimationSequence;

	Sequence->m_Scene = Scene;

	char	FullPath[MAX_PATH] = {};

	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, FileName);

	if (!Sequence->CreateSequenceMultibyte(FullPath))
	{
		SAFE_DELETE(Sequence);
		return false;
	}

	Sequence->m_Loop = Loop;
	Sequence->SetName(Name);

	m_mapSequence.insert(std::make_pair(Name, Sequence));

	return true;
}

bool CAnimationManager3D::LoadAnimationSequenceFullPathMultibyte(bool Loop, const std::string& Name,
	const char* FullPath, class CScene* Scene)
{
	if (FindAnimationSequence(Name))
	{
		return true;
	}

	CAnimationSequence* Sequence = new CAnimationSequence;

	Sequence->m_Scene = Scene;

	if (!Sequence->CreateSequenceMultibyte(FullPath))
	{
		SAFE_DELETE(Sequence);
		return false;
	}

	Sequence->m_Loop = Loop;
	Sequence->SetName(Name);

	m_mapSequence.insert(std::make_pair(Name, Sequence));

	return true;
}

CAnimationSequence* CAnimationManager3D::FindAnimationSequence(const std::string& Name)
{
	auto	iter = m_mapSequence.find(Name);

	if (iter == m_mapSequence.end())
		return nullptr;

	return iter->second;
}

void CAnimationManager3D::ReleaseSequence(const std::string& Name)
{
	auto	iter = m_mapSequence.find(Name);

	if (iter != m_mapSequence.end())
	{
		if (iter->second->GetRefCount() == 1)
			m_mapSequence.erase(iter);
	}
}

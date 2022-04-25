#pragma once

#include "AnimationSequence.h"

class CAnimationManager3D
{
	friend class CResourceManager;

private:
	CAnimationManager3D();
	~CAnimationManager3D();

private:
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence>>	m_mapSequence;

public:
	bool Init();
	bool LoadAnimationSequence(const std::string& Name, bool Loop,
		struct _tagFbxAnimationClip* Clip, class CScene* Scene = nullptr);
	bool LoadAnimationSequence(const std::string& Name, bool Loop,
		int StartFrame, int EndFrame, float PlayTime,
		const std::vector<BoneKeyFrame*>& vecFrame, class CScene* Scene = nullptr);
	bool LoadAnimationSequence(bool Loop, const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = ANIMATION_PATH, class CScene* Scene = nullptr);
	bool LoadAnimationSequenceFullPath(bool Loop, const std::string& Name, const TCHAR* FullPath, class CScene* Scene = nullptr);
	bool LoadAnimationSequenceMultibyte(bool Loop, const std::string& Name, const char* FileName,
		const std::string& PathName = ANIMATION_PATH, class CScene* Scene = nullptr);
	bool LoadAnimationSequenceFullPathMultibyte(bool Loop, const std::string& Name, const char* FullPath, class CScene* Scene = nullptr);
	CAnimationSequence* FindAnimationSequence(const std::string& Name);
	void ReleaseSequence(const std::string& Name);
};


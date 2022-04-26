#pragma once

#include "../../Ref.h"

struct KeyFrame
{
	double	dTime;
	Vector3	vPos;
	Vector3	vScale;
	Vector4	vRot; // 4원수이다. 이 녀석들을 보간해서 행렬을 만들어낸다.
						// Key Frame 사이를 보간해서 행렬을 만들어내는 원리이다.
};

// 한개의 Bone의 Animation Frame 정보 --> 하나의 Bone 이 여러개의 Frame 정보를 가지고 있다 -- 이것들이 하나의 Bone Motion 
struct BoneKeyFrame
{
	// 몇번째 Bone 인지 
	int		iBoneIndex;

	// ex) 칼을 휘두를 때, Bone 마다, Key Frame 이 다르다.
	// 60 Frame 이라고 한다면, 60 Frame * Bone 개수 만큼의 Key Frame 이 필요하다
	// 각각의 Bone 에 대해서, 각 Frame 때마다, 어느 위치에 Bone 이 있어야 하는지 잡아둬야 하기 때문이다.
	std::vector<KeyFrame*>	vecKeyFrame;
	int			iRefCount;

	BoneKeyFrame()
	{
	}

	~BoneKeyFrame()
	{
		size_t	Size = vecKeyFrame.size();

		for (size_t i = 0; i < Size; ++i)
		{
			SAFE_DELETE(vecKeyFrame[i]);
		}

		vecKeyFrame.clear();
	}
};

struct AnimationFrameTrans
{
	Vector4 vTranslate;
	Vector4 vScale;
	Vector4 qRot;
};

struct AnimationNotify
{
	std::string			Name;
	class CAnimationSequence* pOwner;
	float			Time;
	bool			Call;

	AnimationNotify()
	{
		Call = false;
		Time = 0.f;
	}
};

class CAnimationSequence :
    public CRef
{
	friend class CAnimation;
	friend class CResourceManager;
	friend class CSceneResource;
	friend class CAnimationManager3D;

private:
	CAnimationSequence();
	CAnimationSequence(const CAnimationSequence& Anim);
	~CAnimationSequence();

private:
	class CScene*				m_Scene;
	bool						m_Loop;
	float						m_StartTime; // 어디서부터
	float						m_EndTime;  // 어디까지 Motion 이 진행되는가 
	float						m_TimeLength;
	float						m_FrameTime; // 한 프레임 당의 진행 시간
	float						m_PlayTime;
	float						m_PlayScale; // ex) PlayTime 을 1로 하면, 해당 Motion은 1초 동안 진행된다.
													// 원본 애니메이션 재생 속도 --> PlayScale 2배 -> 2배가 되는 것인가 ..?
	int							m_StartFrame;
	int							m_EndFrame;
	int							m_FrameLength;
	int							m_FrameMode;
	int							m_ChangeFrame;
	bool						m_End;
	// 해당 Animation 과정에서, 각 Bone 들의 Frame 정보 --> vector 형태로 담아두기
	std::vector<BoneKeyFrame*>	m_vecKeyFrame;
	char						m_FullPath[MAX_PATH];
	class CStructuredBuffer* m_KeyFrameBuffer; // Compute Shader 로 돌려줘야 하기 때문에 구조화 버퍼를 세팅할 것이다.

public:
	int GetKeyFrameCount()	const
	{
		return m_FrameLength;
	}

public:
	CAnimationSequence* Clone();
	bool IsSequenceEnd()	const;
	void GetKeyFrame(std::vector<BoneKeyFrame*>& vecFrame);
	bool LoadFbxAnimation(const char* pFullPath, bool bLoop = false);
	bool Save(const TCHAR* pFileName, const std::string& strPathName = MESH_PATH);
	bool SaveMultibyte(const char* pFileName, const std::string& strPathName = MESH_PATH);
	bool SaveFullPath(const TCHAR* pFullPath);
	bool SaveFullPathMultibyte(const char* pFullPath);
	bool Load(const TCHAR* pFileName, const std::string& strPathName = MESH_PATH);
	bool LoadMultibyte(const char* pFileName, const std::string& strPathName = MESH_PATH);
	bool LoadFullPath(const TCHAR* pFullPath);
	bool LoadFullPathMultibyte(const char* pFullPath);

public:
	void SetPlayScale(float fScale);
	void SetPlayTime(float fTime);

public:
	bool CreateSequence(bool bLoop, struct _tagFbxAnimationClip* pClip);
	bool CreateSequence(const std::string& strName, bool bLoop,
		int iStartFrame, int iEndFrame, float fPlayTime,
		const std::vector<BoneKeyFrame*>& vecFrame);
	bool CreateSequence(const TCHAR* pFullPath);
	bool CreateSequenceMultibyte(const char* pFullPath);
	bool CreateSequence(bool bLoop, const std::string& strName, const TCHAR* pFileName,
		const std::string& strPathName = MESH_PATH);
	void SetShader();
	void ResetShader();
	void SetChangeShader();
	void ResetChangeShader();
	void Clear();
	void AddNotify(const std::string& Name, float Time);
};


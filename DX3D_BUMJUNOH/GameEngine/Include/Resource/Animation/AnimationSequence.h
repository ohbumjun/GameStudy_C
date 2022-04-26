#pragma once

#include "../../Ref.h"

struct KeyFrame
{
	double	dTime;
	Vector3	vPos;
	Vector3	vScale;
	Vector4	vRot; // 4�����̴�. �� �༮���� �����ؼ� ����� ������.
						// Key Frame ���̸� �����ؼ� ����� ������ �����̴�.
};

// �Ѱ��� Bone�� Animation Frame ���� --> �ϳ��� Bone �� �������� Frame ������ ������ �ִ� -- �̰͵��� �ϳ��� Bone Motion 
struct BoneKeyFrame
{
	// ���° Bone ���� 
	int		iBoneIndex;

	// ex) Į�� �ֵθ� ��, Bone ����, Key Frame �� �ٸ���.
	// 60 Frame �̶�� �Ѵٸ�, 60 Frame * Bone ���� ��ŭ�� Key Frame �� �ʿ��ϴ�
	// ������ Bone �� ���ؼ�, �� Frame ������, ��� ��ġ�� Bone �� �־�� �ϴ��� ��Ƶ־� �ϱ� �����̴�.
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
	float						m_StartTime; // ��𼭺���
	float						m_EndTime;  // ������ Motion �� ����Ǵ°� 
	float						m_TimeLength;
	float						m_FrameTime; // �� ������ ���� ���� �ð�
	float						m_PlayTime;
	float						m_PlayScale; // ex) PlayTime �� 1�� �ϸ�, �ش� Motion�� 1�� ���� ����ȴ�.
													// ���� �ִϸ��̼� ��� �ӵ� --> PlayScale 2�� -> 2�谡 �Ǵ� ���ΰ� ..?
	int							m_StartFrame;
	int							m_EndFrame;
	int							m_FrameLength;
	int							m_FrameMode;
	int							m_ChangeFrame;
	bool						m_End;
	// �ش� Animation ��������, �� Bone ���� Frame ���� --> vector ���·� ��Ƶα�
	std::vector<BoneKeyFrame*>	m_vecKeyFrame;
	char						m_FullPath[MAX_PATH];
	class CStructuredBuffer* m_KeyFrameBuffer; // Compute Shader �� ������� �ϱ� ������ ����ȭ ���۸� ������ ���̴�.

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


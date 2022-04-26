#pragma once

#include "AnimationSequenceData.h"

class CAnimationSequenceInstance
{
public:
	CAnimationSequenceInstance();
	CAnimationSequenceInstance(const CAnimationSequenceInstance& Anim);
	virtual ~CAnimationSequenceInstance();

protected:
	size_t		m_TypeID;
	// AnimationSequenceInstance 의 경우 (3D) -> Sprite Component 가 아니라
	// Animation Mesh Component 가 들고 있게 세팅할 것이다.
	class CAnimationMeshComponent* m_Owner;
	class CScene* m_Scene;
	std::unordered_map<std::string, CAnimationSequenceData*>	m_mapAnimation;

	// 현재 Animation 정보
	CAnimationSequenceData* m_CurrentAnimation;
	// 바뀌는 Animation 정보 세팅
	CAnimationSequenceData* m_ChangeAnimation;
	bool		m_PlayAnimation;

	// Animation 계산 처리를 하는 계산 셰이더
	CSharedPtr<class CComputeShader>	m_AnimationUpdateShader;

	// Animation 상수 버퍼 
	class CAnimationUpdateConstantBuffer* m_AnimationUpdateCBuffer;

	class CStructuredBuffer* m_OutputBuffer;	// 애니메이션 결과 저장용 버퍼
	class CStructuredBuffer* m_BoneBuffer;

	// 최종 갱신될 Bone Matrix 정보를 담아두기 위한 Vector
	std::vector<Matrix>	m_vecBoneMatrix;

	CSharedPtr<class CSkeleton>	m_Skeleton;

	Vector3	m_BlendScale;
	Vector3	m_BlendPos;
	Vector4	m_BlendRot;
	float	m_GlobalTime;
	float	m_SequenceProgress;
	float	m_ChangeTimeAcc;
	float	m_ChangeTime;


public:
	size_t GetTypeID()	const
	{
		return m_TypeID;
	}

	template <typename T>
	void SetTypeID()
	{
		m_TypeID = typeid(T).hash_code();
	}

	void SetScene(class CScene* Scene)
	{
		m_Scene = Scene;
	}

	void SetOwner(class CAnimationMeshComponent* Owner)
	{
		m_Owner = Owner;
	}

	int GetAnimationCount()	const
	{
		return (int)m_mapAnimation.size();
	}

	void Play()
	{
		m_PlayAnimation = true;
	}

	void Stop()
	{
		m_PlayAnimation = false;
	}

	bool IsPlay()	const
	{
		return m_PlayAnimation;
	}

	CAnimationSequenceData* GetCurrentAnimation()	const
	{
		return m_CurrentAnimation;
	}

	void SetSkeleton(class CSkeleton* Skeleton);

public:
	void AddAnimation(const std::string& SequenceName, const std::string& Name, bool Loop = true, float PlayTime = 1.f,
		float PlayScale = 1.f);
	void AddAnimation(const TCHAR* FileName, const std::string& PathName, const std::string& Name, bool Loop = true, float PlayTime = 1.f,
		float PlayScale = 1.f);
	void SetPlayTime(const std::string& Name, float PlayTime);
	void SetPlayScale(const std::string& Name, float PlayScale);
	void SetLoop(const std::string& Name, bool Loop);
	void SetCurrentAnimation(const std::string& Name);
	void ChangeAnimation(const std::string& Name);
	bool CheckCurrentAnimation(const std::string& Name);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	void SetShader();
	void ResetShader();
	virtual CAnimationSequenceInstance* Clone();
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private:
	CAnimationSequenceData* FindAnimation(const std::string& Name);

public:
	template <typename T>
	void SetEndFunction(const std::string& Name, T* Obj, void (T::* Func)())
	{
		CAnimationSequence2DData* Data = FindAnimation(Name);

		if (!Data)
			return;

		Data->SetEndFunction<T>(Obj, Func);
	}

	template <typename T>
	void AddNotify(const std::string& Name, const std::string& NotifyName, int Frame,
		T* Obj, void (T::* Func)())
	{
		CAnimationSequence2DData* Data = FindAnimation(Name);

		if (!Data)
			return;

		Data->AddNotify<T>(NotifyName, Frame, Obj, Func);
	}
};


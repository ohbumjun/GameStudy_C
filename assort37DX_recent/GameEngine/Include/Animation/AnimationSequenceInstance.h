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

	// 현재, 다음 Animation 정보
	std::unordered_map<std::string, CAnimationSequenceData*>	m_mapAnimation;
	CAnimationSequenceData* m_CurrentAnimation;
	CAnimationSequenceData* m_ChangeAnimation;
	bool		m_PlayAnimation;

	// Animation 계산 처리를 하는 계산 셰이더
	CSharedPtr<class CComputeShader>	m_AnimationUpdateShader;
	class CAnimationUpdateConstantBuffer* m_AnimationUpdateCBuffer;

	// Bone 개수만큼의 데이터 개수를 Shader 측에 넘기는 구조화 버퍼 2개가 필요하다.
	// 애니메이션 결과 저장용 버퍼 -> 즉, 1) Offset 행렬 (각 Bone 으로의 Bone Space 변환) + 2) 각 Bone 의 Animation 변환 행렬 정보
	class CStructuredBuffer* m_OutputBuffer;
	// 각 Bone 의 최종 Animation 변환 행렬 정보들 만을, 담은 행렬 Vector
	class CStructuredBuffer* m_BoneBuffer;

	// 구조화 버퍼 내용을 복사해서 넣어주는 버퍼
	ID3D11Buffer* m_BoneDataBuffer;

	// GPU 측에서 최종 갱신된 Bone Animation 변환 행렬 정보들을 CopyResource 를 통해 담아올 변수 
	std::vector<Matrix>	m_vecBoneMatrix;

	CSharedPtr<class CSkeleton>	m_Skeleton;

	// Animation 전환 시에 ex) Idle --> Attack
// 크기, 위치, 회전 등을 보간해야 한다.
// Rotation 정보의 경우, 구면 보간을 위해 ..? 4원수 형태로 값을 들고 있게 세팅할 것이다.
	Vector3	m_BlendScale;
	Vector3	m_BlendPos;
	Vector4	m_BlendRot;

	// 전체 Animation 이 흐른 절대적인 시간 ex) 3초, 4초
	float	m_GlobalTime;

	// 현재 Animation 의 진행률 (0 ~ 1 사이의 값)
	float	m_SequenceProgress;

	// Change Time 누적 진행률 -> ex) 2.5초를 세팅하면, 2.5초 동안 부드럽게 전환되도록 세팅한 것이다.
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
		CAnimationSequenceData* Data = FindAnimation(Name);

		if (!Data)
			return;

		Data->SetEndFunction<T>(Obj, Func);
	}

	template <typename T>
	void AddNotify(const std::string& Name, const std::string& NotifyName, int Frame,
		T* Obj, void (T::* Func)())
	{
		CAnimationSequenceData* Data = FindAnimation(Name);

		if (!Data)
			return;

		Data->AddNotify<T>(NotifyName, Frame, Obj, Func);
	}
};


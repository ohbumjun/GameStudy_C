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
	// AnimationSequenceInstance �� ��� (3D) -> Sprite Component �� �ƴ϶�
// Animation Mesh Component �� ��� �ְ� ������ ���̴�.
	class CAnimationMeshComponent* m_Owner;
	class CScene* m_Scene;

	// ����, ���� Animation ����
	std::unordered_map<std::string, CAnimationSequenceData*>	m_mapAnimation;
	CAnimationSequenceData* m_CurrentAnimation;
	CAnimationSequenceData* m_ChangeAnimation;
	bool		m_PlayAnimation;

	// Animation ��� ó���� �ϴ� ��� ���̴�
	CSharedPtr<class CComputeShader>	m_AnimationUpdateShader;
	class CAnimationUpdateConstantBuffer* m_AnimationUpdateCBuffer;

	// Bone ������ŭ�� ������ ������ Shader ���� �ѱ�� ����ȭ ���� 2���� �ʿ��ϴ�.
	// �ִϸ��̼� ��� ����� ���� -> ��, 1) Offset ��� (�� Bone ������ Bone Space ��ȯ) + 2) �� Bone �� Animation ��ȯ ��� ����
	class CStructuredBuffer* m_OutputBuffer;
	// �� Bone �� ���� Animation ��ȯ ��� ������ ����, ���� ��� Vector
	class CStructuredBuffer* m_BoneBuffer;

	// ����ȭ ���� ������ �����ؼ� �־��ִ� ����
	ID3D11Buffer* m_BoneDataBuffer;

	// GPU ������ ���� ���ŵ� Bone Animation ��ȯ ��� �������� CopyResource �� ���� ��ƿ� ���� 
	std::vector<Matrix>	m_vecBoneMatrix;

	CSharedPtr<class CSkeleton>	m_Skeleton;

	// Animation ��ȯ �ÿ� ex) Idle --> Attack
// ũ��, ��ġ, ȸ�� ���� �����ؾ� �Ѵ�.
// Rotation ������ ���, ���� ������ ���� ..? 4���� ���·� ���� ��� �ְ� ������ ���̴�.
	Vector3	m_BlendScale;
	Vector3	m_BlendPos;
	Vector4	m_BlendRot;

	// ��ü Animation �� �帥 �������� �ð� ex) 3��, 4��
	float	m_GlobalTime;

	// ���� Animation �� ����� (0 ~ 1 ������ ��)
	float	m_SequenceProgress;

	// Change Time ���� ����� -> ex) 2.5�ʸ� �����ϸ�, 2.5�� ���� �ε巴�� ��ȯ�ǵ��� ������ ���̴�.
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


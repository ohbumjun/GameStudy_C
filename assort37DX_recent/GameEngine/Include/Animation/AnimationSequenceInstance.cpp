
#include "AnimationSequenceInstance.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Component/AnimationMeshComponent.h"
#include "../Resource/Animation/AnimationSequence.h"
#include "../Resource/Shader/AnimationUpdateConstantBuffer.h"
#include "../Resource/Shader/AnimationUpdateShader.h"
#include "../Resource/Animation/Skeleton.h"
#include "../Resource/Shader/StructuredBuffer.h"
#include "../Device.h"

CAnimationSequenceInstance::CAnimationSequenceInstance() :
	m_Scene(nullptr),
	m_Owner(nullptr),
	m_PlayAnimation(true),
	m_BoneDataBuffer(nullptr),
	m_GlobalTime(0.f),
	m_SequenceProgress(0.f),
	m_ChangeTimeAcc(0.f),
	m_ChangeTime(0.2f)
{
	SetTypeID<CAnimationSequenceInstance>();
}

CAnimationSequenceInstance::CAnimationSequenceInstance(const CAnimationSequenceInstance& Anim)
{
	m_PlayAnimation = Anim.m_PlayAnimation;

	m_GlobalTime = 0.f;
	m_SequenceProgress = 0.f;
	m_ChangeTimeAcc = 0.f;
	m_ChangeTime = Anim.m_ChangeTime;

	m_AnimationUpdateShader = Anim.m_AnimationUpdateShader;

	if (Anim.m_AnimationUpdateCBuffer)
		m_AnimationUpdateCBuffer = Anim.m_AnimationUpdateCBuffer->Clone();

	if (Anim.m_OutputBuffer)
		m_OutputBuffer = Anim.m_OutputBuffer->Clone();

	if (Anim.m_BoneBuffer)
		m_BoneBuffer = Anim.m_BoneBuffer->Clone();

	m_Skeleton = Anim.m_Skeleton;



	m_mapAnimation.clear();

	auto	iter = Anim.m_mapAnimation.begin();
	auto	iterEnd = Anim.m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		CAnimationSequenceData* Data = new CAnimationSequenceData;

		Data->m_Sequence = iter->second->m_Sequence;
		Data->m_Name = iter->second->m_Name;
		Data->m_Loop = iter->second->m_Loop;
		Data->m_PlayTime = iter->second->m_PlayTime;
		Data->m_PlayScale = iter->second->m_PlayScale;

		if (Anim.m_CurrentAnimation->m_Name == Data->m_Name)
			m_CurrentAnimation = Data;

		m_mapAnimation.insert(std::make_pair(iter->first, Data));
	}

	// CopyResource 를 이용해서, 구조화 버퍼에 있는 것을 복사해서 넣어주는 개념이다.
// 자. 우리는 Shader 측에 넘겨준 m_vecBoneMatrix 정보를 cpu 쪽으로 가져와야 한다.
	D3D11_BUFFER_DESC	Desc = {};

	Desc.ByteWidth = sizeof(Matrix) * (unsigned int)m_Skeleton->GetBoneCount();

	// Usage 는 CPU 에서 GPU 자원을 읽어온 후, 저장용. 이므로, Staging 처리를 한다.
	Desc.Usage = D3D11_USAGE_STAGING;

	// CPU 에서 읽기가 가능해야 한다.
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	CDevice::GetInst()->GetDevice()->CreateBuffer(&Desc, nullptr, &m_BoneDataBuffer);


}

CAnimationSequenceInstance::~CAnimationSequenceInstance()
{
	SAFE_RELEASE(m_BoneDataBuffer);
	SAFE_DELETE(m_OutputBuffer);
	SAFE_DELETE(m_BoneBuffer);
	SAFE_DELETE(m_AnimationUpdateCBuffer);

	auto	iter = m_mapAnimation.begin();
	auto	iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

void CAnimationSequenceInstance::SetSkeleton(CSkeleton* Skeleton)
{
	m_Skeleton = Skeleton;
}

void CAnimationSequenceInstance::AddAnimation(const std::string& SequenceName,
	const std::string& Name, bool Loop,
	float PlayTime, float PlayScale)
{
	CAnimationSequenceData* Anim = FindAnimation(Name);

	if (Anim)
		return;

	CAnimationSequence* Sequence = nullptr;

	if (m_Scene)
	{
		Sequence = m_Scene->GetResource()->FindAnimationSequence(SequenceName);
	}

	else
	{
		Sequence = CResourceManager::GetInst()->FindAnimationSequence(SequenceName);
	}

	if (!Sequence)
		return;

	Anim = new CAnimationSequenceData;

	Anim->m_Sequence = Sequence;
	Anim->m_Name = Name;
	Anim->m_Loop = Loop;
	Anim->m_PlayTime = PlayTime; // Sequence->m_PlayTime;
	Anim->m_PlayScale = PlayScale;
	Anim->m_FrameTime = PlayTime / Sequence->GetKeyFrameCount(); //  Sequence->m_PlayTime / Sequence->GetKeyFrameCount();

	if (m_mapAnimation.empty())
	{
		m_CurrentAnimation = Anim;
	}

	m_mapAnimation.insert(std::make_pair(Name, Anim));
}

void CAnimationSequenceInstance::AddAnimation(const TCHAR* FileName,
	const std::string& PathName, const std::string& Name,
	bool Loop, float PlayTime, float PlayScale)
{
	CAnimationSequenceData* Anim = FindAnimation(Name);

	if (Anim)
		return;

	char	FileNameMultibyte[256] = {};

	int	Length = WideCharToMultiByte(CP_ACP, 0, FileName, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, FileName, -1, FileNameMultibyte, Length, 0, 0);

	CAnimationSequence* Sequence = nullptr;

	if (m_Scene)
	{
		std::string	SequenceName;

		m_Scene->GetResource()->LoadAnimationSequenceMultibyte(Loop, SequenceName, FileNameMultibyte);
		Sequence = m_Scene->GetResource()->FindAnimationSequence(SequenceName);
	}

	else
	{
		std::string	SequenceName;

		CResourceManager::GetInst()->LoadAnimationSequenceMultibyte(Loop, SequenceName, FileNameMultibyte);
		Sequence = CResourceManager::GetInst()->FindAnimationSequence(SequenceName);
	}

	if (!Sequence)
		return;

	Anim = new CAnimationSequenceData;

	Anim->m_Sequence = Sequence;
	Anim->m_Name = Name;
	Anim->m_Loop = Loop;
	Anim->m_PlayTime = PlayTime;
	Anim->m_PlayScale = PlayScale;
	Anim->m_FrameTime = PlayTime / Sequence->GetKeyFrameCount();

	if (m_mapAnimation.empty())
	{
		m_CurrentAnimation = Anim;
	}

	m_mapAnimation.insert(std::make_pair(Name, Anim));
}

void CAnimationSequenceInstance::SetPlayTime(const std::string& Name, float PlayTime)
{
	CAnimationSequenceData* Anim = FindAnimation(Name);

	if (!Anim)
		return;

	Anim->m_PlayTime = PlayTime;
}

void CAnimationSequenceInstance::SetPlayScale(const std::string& Name, float PlayScale)
{
	CAnimationSequenceData* Anim = FindAnimation(Name);

	if (!Anim)
		return;

	Anim->m_PlayScale = PlayScale;
}

void CAnimationSequenceInstance::SetLoop(const std::string& Name, bool Loop)
{
	CAnimationSequenceData* Anim = FindAnimation(Name);

	if (!Anim)
		return;

	Anim->m_Loop = Loop;
}

void CAnimationSequenceInstance::SetCurrentAnimation(const std::string& Name)
{
	m_CurrentAnimation = FindAnimation(Name);

	if (!m_CurrentAnimation)
		return;

	m_CurrentAnimation->m_Time = 0.f;

	size_t Size = m_CurrentAnimation->m_vecNotify.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_CurrentAnimation->m_vecNotify[i]->Call = false;
	}
}

void CAnimationSequenceInstance::ChangeAnimation(const std::string& Name)
{
	if (m_CurrentAnimation->m_Name == Name)
		return;

	//m_CurrentAnimation->m_Time = 0.f;

	size_t	Size = m_CurrentAnimation->m_vecNotify.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_CurrentAnimation->m_vecNotify[i]->Call = false;
	}

	m_ChangeAnimation = FindAnimation(Name);
	m_ChangeAnimation->m_Time = 0.f;
}

bool CAnimationSequenceInstance::CheckCurrentAnimation(const std::string& Name)
{
	return m_CurrentAnimation->m_Name == Name;
}

void CAnimationSequenceInstance::Start()
{
	if (m_Scene)
	{
		m_AnimationUpdateShader = (CComputeShader*)m_Scene->GetResource()->FindShader("AnimationUpdateShader");
	}

	else
	{
		m_AnimationUpdateShader = (CComputeShader*)CResourceManager::GetInst()->FindShader("AnimationUpdateShader");
	}
	
	m_AnimationUpdateCBuffer = new CAnimationUpdateConstantBuffer;

	m_AnimationUpdateCBuffer->Init();

	m_vecBoneMatrix.resize(m_Skeleton->GetBoneCount());

	m_OutputBuffer = new CStructuredBuffer;

	m_OutputBuffer->Init("OutputBone", sizeof(Matrix),
		(unsigned int)m_Skeleton->GetBoneCount(), 0);

	m_BoneBuffer = new CStructuredBuffer;

	m_BoneBuffer->Init("OutputBone", sizeof(Matrix),
		(unsigned int)m_Skeleton->GetBoneCount(), 1);

	D3D11_BUFFER_DESC	Desc = {};

	Desc.ByteWidth = sizeof(Matrix) * (unsigned int)m_Skeleton->GetBoneCount();
	Desc.Usage = D3D11_USAGE_STAGING;
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	CDevice::GetInst()->GetDevice()->CreateBuffer(&Desc, nullptr, &m_BoneDataBuffer);
}

bool CAnimationSequenceInstance::Init()
{
	return true;
}

void CAnimationSequenceInstance::Update(float DeltaTime)
{
	if (!m_Skeleton || !m_CurrentAnimation || !m_PlayAnimation || m_CurrentAnimation->m_Sequence->GetKeyFrameCount() == 0)
		return;

	m_GlobalTime += DeltaTime * m_CurrentAnimation->m_PlayScale;

	bool	Change = false;
	bool	ChangeEnd = false;
	bool	AnimEnd = false;

	// 만약 현재 Animation 이 바뀌어야 한다면
	// 바로 짠 ! 하고 바뀌는 것이 아니다. 일정 시간이 지난 후에 바뀌는 식이다.
	if (m_ChangeAnimation)
	{
		Change = true;
		m_ChangeTimeAcc += DeltaTime;

		// m_ChangeTime 가 m_ChangeTimeAcc 를 도달할 때 까지 기다려야 한다.
		if (m_ChangeTimeAcc >= m_ChangeTime)
		{
			m_ChangeTimeAcc = m_ChangeTime;
			ChangeEnd = true;
		}
	}

	else
	{
		m_SequenceProgress = m_GlobalTime / m_CurrentAnimation->m_PlayTime;

		// m_GlobalTime 은  m_CurrentAnimation->m_PlayTime 을 넘어가지 않는다.
		// m_CurrentAnimation->m_Sequence->m_StartTime, m_EndTime 은 AnimationSequence Load 과정에서 세팅될 텐데
		// 대체로 m_StartTime 은 0, m_EndTime은 m_PlayTime 으로 세팅되게 할 것이다.
		if (m_GlobalTime >= m_CurrentAnimation->m_PlayTime)
		{
			//m_GlobalTime -= m_CurrentAnimation->m_PlayTime;
			m_GlobalTime = m_CurrentAnimation->m_PlayTime;

			AnimEnd = true;
		}

		m_AnimationUpdateCBuffer->SetChangeAnimation(false);
		m_AnimationUpdateCBuffer->SetChangeRatio(0.f);
		m_AnimationUpdateCBuffer->SetChangeFrameCount(0);

		float	AnimationTime = m_GlobalTime + m_CurrentAnimation->m_Sequence->m_StartTime;

		int	StartFrame = 0;

		// Frame 개수 --> 마지막 Frame의 Idx 는 EndFrame - 1 이 될 것이다.
		int	EndFrame = m_CurrentAnimation->m_Sequence->m_FrameLength;

		// m_FrameTime 은 한 프레임이 흐르는데 걸리는 시간
		// int	FrameIndex = (int)(AnimationTime / m_CurrentAnimation->m_Sequence->m_FrameTime);
		int	FrameIndex = (int)(AnimationTime / m_CurrentAnimation->m_FrameTime);
		int	NextFrameIndex = FrameIndex + 1;

		if (FrameIndex >= m_CurrentAnimation->m_Sequence->m_FrameLength)
			FrameIndex = m_CurrentAnimation->m_Sequence->m_FrameLength - 1;

		if (NextFrameIndex >= EndFrame)
			NextFrameIndex = StartFrame;

		// 현재 프레임 시간 중. 실제 흐른 시간의 비율을 구할 것이다.
		// float	Ratio = (AnimationTime - m_CurrentAnimation->m_Sequence->m_FrameTime * FrameIndex) / m_CurrentAnimation->m_Sequence->m_FrameTime;
		float	Ratio = (AnimationTime - m_CurrentAnimation->m_FrameTime * FrameIndex) / m_CurrentAnimation->m_Sequence->m_FrameTime;

		// Frame 개수
		m_AnimationUpdateCBuffer->SetFrameCount(EndFrame);
		// 현재 Frame Idx
		m_AnimationUpdateCBuffer->SetCurrentFrame(FrameIndex);
		// 다음 Frame Idx
		m_AnimationUpdateCBuffer->SetNextFrame(NextFrameIndex);
		// 현재 프레임 시간 중 흐른 프레임 비율
		m_AnimationUpdateCBuffer->SetRatio(Ratio);
		// Bone 개수
		m_AnimationUpdateCBuffer->SetBoneCount((int)m_Skeleton->GetBoneCount());

		size_t	Size = m_CurrentAnimation->m_vecNotify.size();

		for (size_t i = 0; i < Size; ++i)
		{
			if (!m_CurrentAnimation->m_vecNotify[i]->Call &&
				m_CurrentAnimation->m_vecNotify[i]->Frame == FrameIndex)
			{
				m_CurrentAnimation->m_vecNotify[i]->Call = true;
				m_CurrentAnimation->m_vecNotify[i]->Function();
			}
		}

		// 만약 Animation 이 끝났다면
		if (AnimEnd)
		{
			if (m_CurrentAnimation->m_EndFunction)
				m_CurrentAnimation->m_EndFunction();

			if (m_CurrentAnimation->m_Loop)
			{
				m_GlobalTime = 0.f;
				size_t	Size = m_CurrentAnimation->m_vecNotify.size();

				for (size_t i = 0; i < Size; ++i)
				{
					m_CurrentAnimation->m_vecNotify[i]->Call = false;
				}
			}
		}
	}

	// 만약 지금 Animation이 변하는 상태라면, 
	if (Change)
	{
		// 총 Change Time 중에서, 흐른 시간의 비율
		m_AnimationUpdateCBuffer->SetChangeRatio(m_ChangeTimeAcc / m_ChangeTime);
		m_AnimationUpdateCBuffer->SetChangeAnimation(true);
		// 그 다음 Animation 의 Frame 개수를 넘겨준다.
		m_AnimationUpdateCBuffer->SetChangeFrameCount(m_ChangeAnimation->m_Sequence->m_FrameLength);

		// 그 다음 Animation 의 m_KeyFrameBuffer 구조화 버퍼가 Shader 측에 넘어가게 될 것이다.
		// 그러면, AnimationUpdate.fx 코드 상에서 "g_ChangeFrameTransArray" 배열 정보에는, 다음 Animation 이 진행되는데에 있어
		// 모든 Bone 의 모든 KeyFrame 정보들이 1차원 배열로 한꺼번에 세팅된 형태로 들어가 있게 될 것이다.
		m_ChangeAnimation->m_Sequence->SetChangeShader();
	}

	// 첫번째 Row Idx 를 뽑아오도록 세팅한다.
	m_AnimationUpdateCBuffer->SetRowIndex(0);

	// GPU가 각 Bone 의 최종 변환 행렬 정보를 계산한 이후, 해당 결과를 담을 구조화 버퍼 정보를 넘겨준다.
	// AnimationUpdate.fx 코드에서 g_BoneMatrixArray 배열에 해당 정보가 세팅될 것이다.
	m_OutputBuffer->SetShader();

	// m_AnimationUpdateCBuffer 해당 상수 버퍼의 내용을 Shader 측으로 넘겨준다.
	m_AnimationUpdateCBuffer->UpdateCBuffer();

	// 각 Bone 으로의 최초 Bone Space 변환 행렬 정보를 넘겨준다.
	// 이녀석은 최초 Setting 할 때에만, 실제 관련 정보를 UpdateBuffer 함수를 통해 Memory 에 Write 을 해주고
	// 그 다음부터는, 그냥 해당 구조화 버퍼의 셰이더 객체를 만들어주기만 한다.
	// 즉, 처음 딱 한번만 계산하고, 이후에는 계산하지 않는다.
	m_Skeleton->SetShader();

	m_BoneBuffer->SetShader();

	// 현재 Animation 에서, 모든 Bone 에 대한 모든 Frame 정보를 1차원 배열에 담은 정보를,
	// AnimationUpdate.fx 측의 "g_FrameTransArray" 배열에 세팅한다.
	m_CurrentAnimation->m_Sequence->SetShader();

	// Thread 들을 동작시킨다.
	unsigned int GroupX = (unsigned int)m_Skeleton->GetBoneCount() / 256 + 1;

	m_AnimationUpdateShader->Excute(GroupX, 1, 1);

	m_BoneBuffer->ResetShader();

	m_CurrentAnimation->m_Sequence->ResetShader();

	m_Skeleton->ResetShader();

	m_OutputBuffer->ResetShader();

	if (Change)
	{
		m_ChangeAnimation->m_Sequence->ResetChangeShader();
	}

	if (ChangeEnd)
	{
		m_CurrentAnimation = m_ChangeAnimation;
		m_ChangeAnimation = nullptr;
		m_ChangeTimeAcc = 0.f;
		m_GlobalTime = 0.f;
	}

	// 구조화 버퍼에 있는 본 정보를 DataBuffer로 복사한다.
	// 구조화 버퍼는, 처음에 Usage 를 CPU가 읽을 수 없는 Usage 로 세팅해서 넘겨주었기 때문에
	// CPU 측에서 읽을 수 없다.
	// 따라서, 중간 버퍼를 만들어서, 구조화 버퍼의 메모리 내용을 여기에다가 복사하고
	// 해당 버퍼로 부터 CPU가 읽어서, m_vecBoneMatrix 에 읽은 내용을 세팅하는 것이다.
	CDevice::GetInst()->GetContext()->CopyResource(m_BoneDataBuffer, m_BoneBuffer->GetBuffer());

	D3D11_MAPPED_SUBRESOURCE	Map = {};

	CDevice::GetInst()->GetContext()->Map(m_BoneDataBuffer, 0, D3D11_MAP_READ, 0, &Map);

	memcpy(&m_vecBoneMatrix[0], Map.pData, sizeof(Matrix)* m_vecBoneMatrix.size());;

	CDevice::GetInst()->GetContext()->Unmap(m_BoneDataBuffer, 0);
	m_Skeleton->Update(DeltaTime, m_vecBoneMatrix, m_Owner->GetWorldMatrix());
}

void CAnimationSequenceInstance::SetShader()
{
	// AnimationUpdate.fx 에서 106번 레지스터에, Shader Resource View 형태로 넘겨준다.
	// Skinning 처리를 위함이다.
	m_OutputBuffer->SetShader(106, (int)Buffer_Shader_Type::Vertex);
}

void CAnimationSequenceInstance::ResetShader()
{
	m_OutputBuffer->ResetShader(106, (int)Buffer_Shader_Type::Vertex);
}

CAnimationSequenceInstance* CAnimationSequenceInstance::Clone()
{
	return new CAnimationSequenceInstance(*this);
}

void CAnimationSequenceInstance::Save(FILE* File)
{
	int	AnimCount = (int)m_mapAnimation.size();
	fwrite(&AnimCount, sizeof(int), 1, File);

	auto	iter = m_mapAnimation.begin();
	auto	iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		int	Length = (int)iter->first.length();
		fwrite(&Length, sizeof(int), 1, File);
		fwrite(iter->first.c_str(), sizeof(char), Length, File);

		iter->second->Save(File);
	}

	const std::string& CurrentName = m_CurrentAnimation->GetName();
	int	Length = (int)CurrentName.length();
	fwrite(&Length, sizeof(int), 1, File);
	fwrite(CurrentName.c_str(), sizeof(char), Length, File);

	fwrite(&m_PlayAnimation, sizeof(bool), 1, File);
}

void CAnimationSequenceInstance::Load(FILE* File)
{
	int	AnimCount = 0;
	fread(&AnimCount, sizeof(int), 1, File);

	for (int i = 0; i < AnimCount; ++i)
	{
		int	Length = 0;
		char	AnimName[256] = {};

		fread(&Length, sizeof(int), 1, File);
		fread(AnimName, sizeof(char), Length, File);

		CAnimationSequenceData* Data = new CAnimationSequenceData;

		Data->Load(File);

		if (m_Scene)
		{
			Data->m_Sequence = m_Scene->GetResource()->FindAnimationSequence(Data->m_SequenceName);
		}

		else
		{
			Data->m_Sequence = CResourceManager::GetInst()->FindAnimationSequence(Data->m_SequenceName);
		}

		m_mapAnimation.insert(std::make_pair(AnimName, Data));
	}

	int	Length = 0;
	char	CurrentName[256] = {};

	fread(&Length, sizeof(int), 1, File);
	fread(CurrentName, sizeof(char), Length, File);

	m_CurrentAnimation = FindAnimation(CurrentName);

	fread(&m_PlayAnimation, sizeof(bool), 1, File);


	//if (m_Scene)
	//	m_CBuffer = m_Scene->GetResource()->GetAnimation2DCBuffer();
}

CAnimationSequenceData* CAnimationSequenceInstance::FindAnimation(const std::string& Name)
{
	auto	iter = m_mapAnimation.find(Name);

	if (iter == m_mapAnimation.end())
		return nullptr;

	return iter->second;
}

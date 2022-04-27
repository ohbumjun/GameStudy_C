
#include "AnimationSequence.h"
#include "FBXLoader.h"
#include "../../PathManager.h"
#include "../Shader/StructuredBuffer.h"

CAnimationSequence::CAnimationSequence() :
	m_Scene(nullptr),
	m_End(false),
	m_KeyFrameBuffer(nullptr),
	m_PlayScale(1.f),
	m_PlayTime(1.f),
	m_Loop(false),
	m_StartTime(0.f),
	m_EndTime(0.f),
	m_TimeLength(0.f),
	m_FrameTime(0.f),
	m_StartFrame(0),
	m_EndFrame(0),
	m_FrameLength(0),
	m_FrameMode(0),
	m_ChangeFrame(0)
{
}

CAnimationSequence::CAnimationSequence(const CAnimationSequence& anim)
{
	*this = anim;

	m_KeyFrameBuffer = nullptr;

	// m_vecKeyFrame 은 고정이다. 오크 몬스터가 달리는 모션이 있다고 해보자
	// 오크를 100마리 생성해도, 다 같은 모션을 취할 것이다.
	// - 마치 이미지 하나를 같은 Animation 들이 Shared Ptr 형태로 공유했던 것처럼
	// 그러나, 사실 복사 생성자는 ... 굳이 만들 필요가 없을 수도 있다. 굳이 복사하지 않을 경우가 더 많기 때문이다.
	for (size_t i = 0; i < m_vecKeyFrame.size(); ++i)
	{
		++m_vecKeyFrame[i]->iRefCount;
	}
}

CAnimationSequence::~CAnimationSequence()
{
	SAFE_DELETE(m_KeyFrameBuffer);

	for (size_t i = 0; i < m_vecKeyFrame.size(); ++i)
	{
		--m_vecKeyFrame[i]->iRefCount;

		if (m_vecKeyFrame[i]->iRefCount == 0)
			delete	m_vecKeyFrame[i];
	}

	m_vecKeyFrame.clear();
}

CAnimationSequence* CAnimationSequence::Clone()
{
	return new CAnimationSequence(*this);
}

bool CAnimationSequence::IsSequenceEnd() const
{
	return m_End;
}

void CAnimationSequence::GetKeyFrame(std::vector<BoneKeyFrame*>& vecFrame)
{
	vecFrame = m_vecKeyFrame;
}

bool CAnimationSequence::LoadFbxAnimation(const char* pFullPath, bool bLoop)
{
	CFBXLoader	loader;

	// 해당 Full Path 의 파일이, Motion 하나하나를 의미하는 것이다
	loader.LoadFBX(pFullPath, false);

	// 애니메이션 클립을 추가한다.
	const std::vector<PFBXANIMATIONCLIP>* pvecClip = loader.GetClips();

	memset(m_FullPath, 0, MAX_PATH);
	strcpy_s(m_FullPath, pFullPath);

	return CreateSequence(bLoop, pvecClip->front());
}

bool CAnimationSequence::Save(const TCHAR* pFileName,
	const std::string& strPathName)
{
	char	strFileName[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, pFileName, -1, strFileName, lstrlen(pFileName),
		NULL, NULL);
#else
	strcpy_s(strFileName, pFileName);
#endif // UNICODE

	return SaveMultibyte(strFileName, strPathName);
}

bool CAnimationSequence::SaveMultibyte(const char* pFileName,
	const std::string& strPathName)
{
	const PathInfo* pPath = CPathManager::GetInst()->FindPath(strPathName);

	char	strFullPath[MAX_PATH] = {};

	if (pPath)
		strcpy_s(strFullPath, pPath->PathMultibyte);

	strcat_s(strFullPath, pFileName);

	return SaveFullPathMultibyte(strFullPath);
}

bool CAnimationSequence::SaveFullPath(const TCHAR* pFullPath)
{
	char	strFullPath[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, pFullPath, -1, strFullPath, lstrlen(pFullPath),
		NULL, NULL);
#else
	strcpy_s(strFileName, pFileName);
#endif // UNICODE

	return SaveFullPathMultibyte(strFullPath);
}

bool CAnimationSequence::SaveFullPathMultibyte(
	const char* pFullPath)
{
	FILE* pFile = NULL;

	fopen_s(&pFile, pFullPath, "wb");

	if (!pFile)
		return false;

	size_t	iLength = m_Name.length();
	fwrite(&iLength, sizeof(size_t), 1, pFile);
	fwrite(m_Name.c_str(), sizeof(char), iLength, pFile);

	fwrite(&m_Loop, sizeof(bool), 1, pFile);
	fwrite(&m_StartTime, sizeof(float), 1, pFile);
	fwrite(&m_EndTime, sizeof(float), 1, pFile);
	fwrite(&m_TimeLength, sizeof(float), 1, pFile);
	fwrite(&m_FrameTime, sizeof(float), 1, pFile);
	fwrite(&m_PlayTime, sizeof(float), 1, pFile);
	fwrite(&m_PlayScale, sizeof(float), 1, pFile);
	fwrite(&m_StartFrame, sizeof(int), 1, pFile);
	fwrite(&m_EndFrame, sizeof(int), 1, pFile);
	fwrite(&m_FrameLength, sizeof(int), 1, pFile);
	fwrite(&m_FrameMode, sizeof(int), 1, pFile);
	fwrite(&m_ChangeFrame, sizeof(int), 1, pFile);

	size_t	iCount = m_vecKeyFrame.size();

	fwrite(&iCount, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iCount; ++i)
	{
		fwrite(&m_vecKeyFrame[i]->iBoneIndex, sizeof(int), 1,
			pFile);

		size_t	iFrameCount = m_vecKeyFrame[i]->vecKeyFrame.size();

		fwrite(&iFrameCount, sizeof(size_t), 1, pFile);

		for (size_t j = 0; j < iFrameCount; ++j)
		{
			fwrite(&m_vecKeyFrame[i]->vecKeyFrame[j]->dTime, sizeof(double), 1, pFile);
			fwrite(&m_vecKeyFrame[i]->vecKeyFrame[j]->vPos, sizeof(Vector3), 1, pFile);
			fwrite(&m_vecKeyFrame[i]->vecKeyFrame[j]->vScale, sizeof(Vector3), 1, pFile);
			fwrite(&m_vecKeyFrame[i]->vecKeyFrame[j]->vRot, sizeof(Vector4), 1, pFile);
		}
	}

	fclose(pFile);

	return true;
}

bool CAnimationSequence::Load(const TCHAR* pFileName,
	const std::string& strPathName)
{
	char	strFileName[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, pFileName, -1, strFileName, lstrlen(pFileName),
		NULL, NULL);
#else
	strcpy_s(strFileName, pFileName);
#endif // UNICODE

	return LoadMultibyte(strFileName, strPathName);
}

bool CAnimationSequence::LoadMultibyte(const char* pFileName,
	const std::string& strPathName)
{
	const PathInfo* pPath = CPathManager::GetInst()->FindPath(strPathName);

	char	strFullPath[MAX_PATH] = {};

	if (pPath)
		strcpy_s(strFullPath, pPath->PathMultibyte);

	strcat_s(strFullPath, pFileName);

	return LoadFullPathMultibyte(strFullPath);
}

bool CAnimationSequence::LoadFullPath(const TCHAR* pFullPath)
{
	char	strFullPath[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, pFullPath, -1, strFullPath, lstrlen(pFullPath),
		NULL, NULL);
#else
	strcpy_s(strFileName, pFileName);
#endif // UNICODE

	return LoadFullPathMultibyte(strFullPath);
}

bool CAnimationSequence::LoadFullPathMultibyte(const char* pFullPath)
{
	FILE* pFile = NULL;

	fopen_s(&pFile, pFullPath, "rb");

	if (!pFile)
		return false;

	size_t	iLength = 0;
	fread(&iLength, sizeof(size_t), 1, pFile);
	char	strName[256] = {};
	fread(strName, sizeof(char), iLength, pFile);

	SetName(strName);

	bool	bLoop = true;
	fread(&bLoop, sizeof(bool), 1, pFile);
	fread(&m_StartTime, sizeof(float), 1, pFile);
	fread(&m_EndTime, sizeof(float), 1, pFile);
	fread(&m_TimeLength, sizeof(float), 1, pFile);
	fread(&m_FrameTime, sizeof(float), 1, pFile);
	fread(&m_PlayTime, sizeof(float), 1, pFile);
	fread(&m_PlayScale, sizeof(float), 1, pFile);
	fread(&m_StartFrame, sizeof(int), 1, pFile);
	fread(&m_EndFrame, sizeof(int), 1, pFile);
	fread(&m_FrameLength, sizeof(int), 1, pFile);
	fread(&m_FrameMode, sizeof(int), 1, pFile);
	fread(&m_ChangeFrame, sizeof(int), 1, pFile);

	size_t	iCount = 0;

	fread(&iCount, sizeof(size_t), 1, pFile);

	std::vector<AnimationFrameTrans>	vecFrameTrans;
	vecFrameTrans.resize(iCount * m_FrameLength);

	for (size_t i = 0; i < iCount; ++i)
	{
		BoneKeyFrame* pBoneKeyFrame = new BoneKeyFrame;
		m_vecKeyFrame.push_back(pBoneKeyFrame);

		fread(&pBoneKeyFrame->iBoneIndex, sizeof(int), 1,
			pFile);

		size_t	iBoneFrameCount = 0;

		fread(&iBoneFrameCount, sizeof(size_t), 1, pFile);

		for (size_t j = 0; j < iBoneFrameCount; ++j)
		{
			KeyFrame* pKeyFrame = new KeyFrame;
			pBoneKeyFrame->vecKeyFrame.push_back(pKeyFrame);

			fread(&pKeyFrame->dTime, sizeof(double), 1, pFile);
			fread(&pKeyFrame->vPos, sizeof(Vector3), 1, pFile);
			fread(&pKeyFrame->vScale, sizeof(Vector3), 1, pFile);
			fread(&pKeyFrame->vRot, sizeof(Vector4), 1, pFile);

			if (j < m_FrameLength)
			{
				AnimationFrameTrans	tFrame = {};
				tFrame.vScale = Vector4(pKeyFrame->vScale.x, pKeyFrame->vScale.y,
					pKeyFrame->vScale.z, 1.f);
				tFrame.vTranslate = Vector4(pKeyFrame->vPos.x, pKeyFrame->vPos.y,
					pKeyFrame->vPos.z, 1.f);
				tFrame.qRot = pKeyFrame->vRot;

				vecFrameTrans[i * m_FrameLength + j] = tFrame;
			}
		}
	}

	m_KeyFrameBuffer = new CStructuredBuffer;

	m_KeyFrameBuffer->Init("KeyFrameBuffer", sizeof(AnimationFrameTrans),
		vecFrameTrans.size(), 13, true, (int)Buffer_Shader_Type::Compute);

	m_KeyFrameBuffer->UpdateBuffer(&vecFrameTrans[0],
		vecFrameTrans.size());

	fclose(pFile);

	return true;
}

void CAnimationSequence::SetPlayScale(float fScale)
{
	m_PlayScale = fScale;
}

void CAnimationSequence::SetPlayTime(float fTime)
{
	// 해당 Animation의 진행 시간을 세팅할 것이다.
	m_PlayTime = fTime;

	m_EndTime = m_PlayTime;
	m_TimeLength = m_PlayTime;

	// 애니메이션 상에서 한 Frame 의 재생 시간.
	m_FrameTime = m_PlayTime / m_FrameLength;

	// 키 프레임 수만큼 반복하며 각각의 프레임을 보간할 행렬 정보를 (위치, 크기, 회전 정보) 로 뽑아온다.
	// m_vecKeyFrame -> Animation 상, 모든 Bone 들의 Animation 들을 Vector 형태로 저장한 것.
	for (size_t i = 0; i < m_vecKeyFrame.size(); ++i)
	{
		BoneKeyFrame* pBoneKeyFrame = m_vecKeyFrame[i];

		for (size_t j = 0; j < pBoneKeyFrame->vecKeyFrame.size(); ++j)
		{
			KeyFrame* pKeyFrame = pBoneKeyFrame->vecKeyFrame[j];

			pKeyFrame->dTime = j * m_FrameTime;
		}
	}
}

bool CAnimationSequence::CreateSequence(bool bLoop,
	_tagFbxAnimationClip* pClip)
{
	// 인자로 들어온 애니메이션 옵션정보를 설정한다.
	m_Loop = bLoop;
	SetName(pClip->strName);

	m_ChangeFrame = 0;

	switch (pClip->eTimeMode)
	{
		// ex) 24 Frame 짜리 인지, 30Frame 짜리 인지. 를 구분해서 세팅해줄 것이다.
	case FbxTime::eFrames24:
		m_FrameMode = 24;
		break;
	case FbxTime::eFrames30:
		m_FrameMode = 30;
		break;
	case FbxTime::eFrames60:
		m_FrameMode = 60;
		break;
	}

	// FBXANIMATIONCLIP에 있는 starttime 과 endtime 을 이용하여 keyframe 을 얻어온다.
	m_StartFrame = pClip->tStart.GetFrameCount(pClip->eTimeMode);
	m_EndFrame = pClip->tEnd.GetFrameCount(pClip->eTimeMode);

	// 총 프레임 개수
	m_FrameLength = m_EndFrame - m_StartFrame + 1;

	// 시간 정보를 저장해준다.
	m_StartTime = 0.f;
	m_EndTime = m_PlayTime; // 총 애니메이션 진행 시간
	m_TimeLength = m_PlayTime;

	// 애니메이션 상 한 프레임당의 시간
	m_FrameTime = m_PlayTime / m_FrameLength;

	// 해당 Animation 상에서의 모든 Bone 내의 Animation 관련 Frame 을 하나의 일차원 배열로 만들어서 저장할 것이다.
	std::vector<AnimationFrameTrans>	vecFrameTrans;
	vecFrameTrans.resize(pClip->vecBoneKeyFrame.size() * m_FrameLength);

	// 본 3개 키프레임 5개일 경우
	// 본0키0|본0키1|본0키2|본0키3|본0키4|본1키0|본1키1|본1키2|본1키3|본1키4|본2키0|본2키1|본2키2|본2키3|본2키4

	// 키 프레임 수만큼 반복하며 각각의 프레임을 보간할 행렬 정보를 위치, 크기, 회전정보로
	// 뽑아온다.
	for (size_t i = 0; i < pClip->vecBoneKeyFrame.size(); ++i)
	{
		BoneKeyFrame* pBoneKeyFrame = new BoneKeyFrame;

		// 몇번째 Bone 의 Key Frame 정보인지 에 대한 Idx 
		pBoneKeyFrame->iBoneIndex = pClip->vecBoneKeyFrame[i].iBoneIndex;

		// m_vecKeyFrame ? -> 모든 Bone의 KeyFrame 정보 추가 --> Bone 개수만큼 지니고 있게 한다.
		m_vecKeyFrame.push_back(pBoneKeyFrame);

		// 아래부터 키프레임 정보를 저장한다.
		// 해당 Bone 의 KeyFrame 정보들을 세팅해줄 것이다. -> 당연히, 캐릭터 하나의 Full Animation 이 있을 때
		// 캐릭터를 구성하는 모든 Bone 마다, Animation이 다 있을 것이다.
		// 어떤 Bone 은 더 많은 Animation 이 있을 수 있다. 더 많이 움직이니까
		// 혹은, 더 적은 Animation 의 Bone 이 있을 수 있다.더 적게 움직이니까 
		pBoneKeyFrame->vecKeyFrame.reserve(pClip->vecBoneKeyFrame[i].vecKeyFrame.size());

		// 해당 Bone 의 Animation 개수만큼 반복한다.
		for (size_t j = 0; j < pClip->vecBoneKeyFrame[i].vecKeyFrame.size(); ++j)
		{
			KeyFrame* pKeyFrame = new KeyFrame;

			// 시작 시간 ..?
			pKeyFrame->dTime = j * m_FrameTime;

			// 현재 본의 키 프레임에 해당하는 행렬 정보를 얻어온다.
			FbxAMatrix		mat = pClip->vecBoneKeyFrame[i].vecKeyFrame[j].matTransform;

			FbxVector4		vPos, vScale;
			FbxQuaternion	qRot;

			// 행렬로부터 위치, 크기, 회전 정보를 얻어온다.
			vPos = mat.GetT();
			vScale = mat.GetS();
			qRot = mat.GetQ();

			// Pos, Scale 은 3차원 
			pKeyFrame->vScale = Vector3((float)vScale.mData[0], (float)vScale.mData[1],
				(float)vScale.mData[2]);
			pKeyFrame->vPos = Vector3((float)vPos.mData[0], (float)vPos.mData[1],
				(float)vPos.mData[2]);

			// Rot 은 4차원
			pKeyFrame->vRot = Vector4((float)qRot.mData[0], (float)qRot.mData[1],
				(float)qRot.mData[2], (float)qRot.mData[3]);

			// 해당 Bone 의 Key Frame 정보 배열에 Push 해준다.
			pBoneKeyFrame->vecKeyFrame.push_back(pKeyFrame);

			if (j < m_FrameLength)
			{
				AnimationFrameTrans	tFrame = {};
				tFrame.vScale = Vector4(pKeyFrame->vScale.x, pKeyFrame->vScale.y,
					pKeyFrame->vScale.z, 1.f);
				tFrame.vTranslate = Vector4(pKeyFrame->vPos.x, pKeyFrame->vPos.y,
					pKeyFrame->vPos.z, 1.f);
				tFrame.qRot = pKeyFrame->vRot;

				// 일차원 배열 형태로, 모든 Bone의 모든 Frame 정보들을 저장할 것이다.
				// i번째 Bone 의 j번째 Animation 정보를 의미할 것이다.
				vecFrameTrans[i * m_FrameLength + j] = tFrame;
			}

			else
				int a = 10;
		}
	}

	m_KeyFrameBuffer = new CStructuredBuffer;

	m_KeyFrameBuffer->Init("KeyFrameBuffer", sizeof(AnimationFrameTrans),
		vecFrameTrans.size(), 13, true, (int)Buffer_Shader_Type::Compute);

	// 위에서 만들어준 일차원 배열 정보를 Shader 측에 구조화 버퍼 형태로 넘겨줄 것이다.
	m_KeyFrameBuffer->UpdateBuffer(&vecFrameTrans[0],
		vecFrameTrans.size());

	if (strlen(m_FullPath) != 0)
	{
		char	strAnimName[MAX_PATH] = {};

		_splitpath_s(m_FullPath, 0, 0, 0, 0, strAnimName, MAX_PATH, 0, 0);

		char	strAnimPath[MAX_PATH] = {};
		strcpy_s(strAnimPath, m_FullPath);

		int	iLength = strlen(strAnimPath);
		for (int i = iLength - 1; i >= 0; --i)
		{
			// aa/bb.exe 9개, 2번인덱스 3 ~ 8번까지 제거
			if (strAnimPath[i] == '/' || strAnimPath[i] == '\\')
			{
				memset(&strAnimPath[i + 1], 0, sizeof(TCHAR) * (iLength - (i + 1)));
				break;
			}
		}

		// .sqc 라는 형식의 파일로 만들어줄 것이다.
		char	strAnimFullPath[MAX_PATH] = {};
		strcpy_s(strAnimFullPath, strAnimPath);
		strcat_s(strAnimFullPath, strAnimName);
		strcat_s(strAnimFullPath, ".sqc");

		SaveFullPathMultibyte(strAnimFullPath);
	}

	return true;
}

bool CAnimationSequence::CreateSequence(
	const std::string& strName, bool bLoop, int iStartFrame,
	int iEndFrame, float fPlayTime,
	const std::vector<BoneKeyFrame*>& vecFrame)
{
	// 인자로 들어온 애니메이션 옵션정보를 설정한다.
	m_Loop = bLoop;
	SetName(strName);

	m_ChangeFrame = 0;

	// FBXANIMATIONCLIP에 있는 starttime 과 endtime 을 이용하여 keyframe 을 얻어온다.
	m_StartFrame = 0;
	m_EndFrame = iEndFrame - iStartFrame;
	m_FrameLength = m_EndFrame - m_StartFrame + 1;

	m_PlayTime = fPlayTime;
	m_FrameTime = fPlayTime / m_FrameLength;

	// 시간 정보를 저장해준다.
	m_StartTime = 0.f;
	m_EndTime = fPlayTime;
	m_TimeLength = fPlayTime;

	std::vector<AnimationFrameTrans>	vecFrameTrans;
	vecFrameTrans.resize(vecFrame.size() * m_FrameLength);

	// 키 프레임 수만큼 반복하며 각각의 프레임을 보간할 행렬 정보를 위치, 크기, 회전정보로
	// 뽑아온다.
	for (size_t i = 0; i < vecFrame.size(); ++i)
	{
		BoneKeyFrame* pBoneKeyFrame = new BoneKeyFrame;

		pBoneKeyFrame->iBoneIndex = vecFrame[i]->iBoneIndex;

		m_vecKeyFrame.push_back(pBoneKeyFrame);

		// 아래부터 키프레임 정보를 저장한다.
		pBoneKeyFrame->vecKeyFrame.reserve(m_FrameLength);

		if (!vecFrame[i]->vecKeyFrame.empty())
		{
			for (size_t j = 0; j < m_FrameLength; ++j)
			{
				KeyFrame* pKeyFrame = new KeyFrame;

				pKeyFrame->dTime = (j - iStartFrame) * m_FrameTime;
				pKeyFrame->vScale = vecFrame[i]->vecKeyFrame[j]->vScale;
				pKeyFrame->vRot = vecFrame[i]->vecKeyFrame[j]->vRot;
				pKeyFrame->vPos = vecFrame[i]->vecKeyFrame[j]->vPos;

				pBoneKeyFrame->vecKeyFrame.push_back(pKeyFrame);

				if (j < m_FrameLength)
				{
					AnimationFrameTrans	tFrame = {};
					tFrame.vScale = Vector4(pKeyFrame->vScale.x, pKeyFrame->vScale.y,
						pKeyFrame->vScale.z, 1.f);
					tFrame.vTranslate = Vector4(pKeyFrame->vPos.x, pKeyFrame->vPos.y,
						pKeyFrame->vPos.z, 1.f);
					tFrame.qRot = pKeyFrame->vRot;

					vecFrameTrans[i * m_FrameLength + j] = tFrame;
				}

				else
					int a = 10;
			}
		}
	}

	m_KeyFrameBuffer = new CStructuredBuffer;

	m_KeyFrameBuffer->Init("KeyFrameBuffer", sizeof(AnimationFrameTrans),
		vecFrameTrans.size(), 13, true, (int)Buffer_Shader_Type::Compute);

	m_KeyFrameBuffer->UpdateBuffer(&vecFrameTrans[0],
		vecFrameTrans.size());

	return true;
}

bool CAnimationSequence::CreateSequence(const TCHAR* pFullPath)
{
	char	strFullPath[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_UTF8, 0, pFullPath, -1, strFullPath, lstrlen(pFullPath),
		0, 0);
#else
	strcpy_s(strFullPath, pFullPath);
#endif // UNICODE

	return CreateSequenceMultibyte(strFullPath);
}

bool CAnimationSequence::CreateSequenceMultibyte(const char* pFullPath)
{
	char	strExt[_MAX_EXT] = {};

	_splitpath_s(pFullPath, 0, 0, 0, 0, 0, 0, strExt, _MAX_EXT);

	_strupr_s(strExt);

	if (strcmp(strExt, ".FBX") == 0)
	{
		return LoadFbxAnimation(pFullPath);
	}

	return LoadFullPathMultibyte(pFullPath);
}

bool CAnimationSequence::CreateSequence(bool bLoop, const std::string& strName,
	const TCHAR* pFileName, const std::string& strPathName)
{
	SetName(strName);

	m_Loop = bLoop;

	const PathInfo* pPath = CPathManager::GetInst()->FindPath(strPathName);

	TCHAR	strFullPath[MAX_PATH] = {};

	if (pPath)
		lstrcpy(strFullPath, pPath->Path);

	lstrcat(strFullPath, pFileName);

	return CreateSequence(strFullPath);
}

void CAnimationSequence::SetShader()
{
	m_KeyFrameBuffer->SetShader();
}

void CAnimationSequence::ResetShader()
{
	m_KeyFrameBuffer->ResetShader();
}

void CAnimationSequence::SetChangeShader()
{
	// 모든 Bone의 모든 Frame 을 하나의 Vector 로 만든 다음, 그것을 기반으로 만들어낸 구조화 버퍼. 이것을 Shader 측에 넘겨줄 것이다.
	m_KeyFrameBuffer->SetShader(15, (int)Buffer_Shader_Type::Compute);
}

void CAnimationSequence::ResetChangeShader()
{
	m_KeyFrameBuffer->ResetShader(15, (int)Buffer_Shader_Type::Compute);
}

void CAnimationSequence::Clear()
{
}

/*
void CAnimationSequence::AddNotify(const std::string& Name, float Time)
{
	AnimationNotify* pNotify = new AnimationNotify;

	pNotify->Name = Name;
	pNotify->Time = Time;
	pNotify->pOwner = this;

	m_vecNotify.push_back(pNotify);
}
*/
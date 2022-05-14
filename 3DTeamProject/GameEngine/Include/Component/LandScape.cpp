#include "LandScape.h"
#include "../PathManager.h"
#include "../Resource/Mesh/StaticMesh.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Material/Material.h"
#include "../Resource/Shader/LandScapeConstantBuffer.h"

CLandScape::CLandScape() :
	m_CountX(0),
	m_CountZ(0),
	m_CBuffer(nullptr)
{
	SetTypeID<CLandScape>();
	m_Render = true;
}

CLandScape::CLandScape(const CLandScape& com)
{
	m_CountX = com.m_CountX;
	m_CountZ = com.m_CountZ;
	m_Mesh = com.m_Mesh;
	m_vecVtx = com.m_vecVtx;
	m_vecPos = com.m_vecPos;
	m_vecIndex = com.m_vecIndex;

	m_CBuffer = com.m_CBuffer->Clone();
}

CLandScape::~CLandScape()
{
	SAFE_DELETE(m_CBuffer);
}

// 가져온 Height Map 에 기반하여
// 격자 개수만큼의 정점을 지닌 
// Mesh 를 만들어주는 함수
// 각 정점의 위치, Normal, Index, Tagnent, Binormal 정보를 모두 만들어준다.
void CLandScape::CreateLandScape(const std::string& Name, int CountX, int CountZ, const TCHAR* HeightMap, const std::string& PathName)
{
	m_CountX = CountX;
	m_CountZ = CountZ;

	std::vector<float>	vecY;

	if (HeightMap)
	{
		TCHAR	FullPath[MAX_PATH] = {};

		const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

		if (Path)
			lstrcpy(FullPath, Path->Path);
		lstrcat(FullPath, HeightMap);

		ScratchImage	Img;

		char	Ext[_MAX_EXT] = {};
		char	FullPathMultibyte[MAX_PATH] = {};

#ifdef UNICODE

		int	ConvertLength = WideCharToMultiByte(CP_ACP, 0, FullPath, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_ACP, 0, FullPath, -1, FullPathMultibyte, ConvertLength, nullptr, nullptr);

#else

		strcpy_s(FullPathMultibyte, FullPath);

#endif // UNICODE

		_splitpath_s(FullPathMultibyte, nullptr, 0, nullptr, 0, nullptr, 0, Ext, _MAX_EXT);

		_strupr_s(Ext);

		if (strcmp(Ext, ".DDS") == 0)
		{
			if (FAILED(LoadFromDDSFile(FullPath, DDS_FLAGS_NONE, nullptr, Img)))
			{
				return;
			}
		}

		else if (strcmp(Ext, ".TGA") == 0)
		{
			if (FAILED(LoadFromTGAFile(FullPath, nullptr, Img)))
			{
				return;
			}
		}

		else
		{
			if (FAILED(LoadFromWICFile(FullPath, WIC_FLAGS_NONE, nullptr, Img)))
			{
				return;
			}
		}

		// 각 픽셀 값을 꺼내온다. 픽셀 하나하나의 정보를 뽑아와야 한다.
		const Image* PixelInfo = Img.GetImages();

		for (size_t i = 0; i < PixelInfo->height; ++i)
		{
			for (size_t j = 0; j < PixelInfo->width; ++j)
			{
				// 픽셀은 1byte 4개짜리가 모여서 하나의 색상을 만든다.
				// 따라서 Idx * 4 를 해줘야 한다.
				int PixelIndex = (int)i * (int)PixelInfo->width * 4 + (int)j * 4;

				// 이렇게 하면, r,g,b,a 중에서 r 값만 뽑아올 것이다.
				// Height Map 은 Gray Scale 형태로 뽑아올 것이다.
				// 따라서 r,g,b,a 중 어떤 것을 뽑아와도 상관없다.
				// 다시 말하면,애초에 Height Map 자체게, Gray Scale 이고
				// 해당 Pixel 의 r,g,b,a 값은, 원래 높이 정보들을 r,g,b,a 에 동일하게 저장한 형태이므로
				// GrayScale 형태를 띨 수 밖에 없다는 것이다.

				// 그리고 GrayScale 에 저장된 픽셀은 0 에서 255사이의 값이다 (r,g,b,a 자체가 0에서 255 사이의 값)
				// 그런데, 실제 Render 시에 높이 255 는 너무 큰값이다.
				// 따라서, 60으로 나눠줄 것이다 ( 마음대로 세팅해도 되는 값)
				float Y = PixelInfo->pixels[PixelIndex] / 60.f;

				vecY.push_back(Y);
			}
		}
	}
	else
	{
		// 높이 맵이 없다면, 0으로 다 깔아뭉갤 것이다.
		vecY.resize(m_CountX * m_CountZ);
	}

	// 정점 정보 만들기
	for (int i = 0; i < m_CountZ; ++i)
	{
		for (int j = 0; j < m_CountX; ++j)
		{
			// 모든 정점을 순회하면서, 해당 개수만큼 정점을 만들어낼 것이다.
			Vertex3D Vtx = {};

			// x,y,z
			// 왼쪽 위에서부터 내려오므로 i 가 아니라 m_CountZ - i 
			// ex) 1 ~ 100 --> 0 ~ 99 로 만들기 위함 --> 따라서 - 1
			Vtx.Pos = Vector3((float)j, vecY[i * m_CountX + j], (float)m_CountZ - i - 1);
			// UV 의 경우, ex) 0 ~ 99 로 x가 들어갈 때 --> 99일 때 1 이라는 값이 나와야 한다.
			Vtx.UV = Vector2(j / (float)(m_CountX - 1), i / (float)(m_CountZ - 1));

			m_vecVtx.push_back(Vtx);
		}
	}

	// 인덱스 정보 만들기
	Vector3 TrianglePos[3];
	Vector3 Edge1, Edge2;
	unsigned int TriIndex = 0;

	// 삼각형 개수만큼 만들기 
	// 왜 - 1을 해줘서 곱해주는 것인가 ?
	// ex) 5*5 격자 --> 실제 만들어지는 사각형은 4*4 -> 16개
	// ex) N개의 사각형 --> N * 2개의 삼각형 을 순회하면서 색인 버퍼 정보를 만들어줘야 한다.
	m_vecFaceNormal.resize((m_CountX - 1)* (m_CountZ - 1) * 2);

	for (unsigned int i = 0; i < m_CountZ - 1; ++i)
	{
		for (unsigned int j = 0; j < m_CountX - 1; ++j)
		{
			unsigned int Index = i * m_CountX + j;

			// 사각형의 우상단 삼각형 (시계방향 -> 전면 삼각형 형태로 만들기 위하여)
			m_vecIndex.push_back(Index);
			m_vecIndex.push_back(Index + 1);
			m_vecIndex.push_back(Index + m_CountX + 1);

			TrianglePos[0] = m_vecVtx[Index].Pos;
			TrianglePos[1] = m_vecVtx[Index + 1].Pos;
			TrianglePos[2] = m_vecVtx[Index + m_CountX + 1].Pos;

			// 오른손 법칙 --> 우리쪽으로 향하는 법선 벡터를 만들어내기
			Edge1 = TrianglePos[1] - TrianglePos[0];
			Edge2 = TrianglePos[2] - TrianglePos[0];

			Edge1.Normalize();
			Edge2.Normalize();

			m_vecFaceNormal[TriIndex] = Edge1.Cross(Edge2);
			m_vecFaceNormal[TriIndex].Normalize();

			++TriIndex;

			// 사각형의 좌하단 삼각형
			m_vecIndex.push_back(Index);
			m_vecIndex.push_back(Index + m_CountX + 1);
			m_vecIndex.push_back(Index + m_CountX);

			TrianglePos[0] = m_vecVtx[Index].Pos;
			TrianglePos[1] = m_vecVtx[Index + m_CountX + 1].Pos;
			TrianglePos[2] = m_vecVtx[Index + m_CountX].Pos;

			// 오른손 법칙 --> 우리쪽으로 향하는 법선 벡터를 만들어내기
			Edge1 = TrianglePos[1] - TrianglePos[0];
			Edge2 = TrianglePos[2] - TrianglePos[0];

			Edge1.Normalize();
			Edge2.Normalize();

			m_vecFaceNormal[TriIndex] = Edge1.Cross(Edge2);
			m_vecFaceNormal[TriIndex].Normalize();

			++TriIndex;
		}
	}

	ComputeNormal();

	ComputeTangent();

	// 메쉬를 만든다.
	m_Scene->GetResource()->CreateMesh(Mesh_Type::Static, Name,
		&m_vecVtx[0], sizeof(Vertex3D), (int)m_vecVtx.size(),
		D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		&m_vecIndex[0], sizeof(int), (int)m_vecIndex.size(),
		D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);

	m_Mesh = (CStaticMesh*)m_Scene->GetResource()->FindMesh(Name);

}


void CLandScape::SetMaterial(CMaterial* Material, int Index)
{
	m_vecMaterialSlot[Index] = Material->Clone();

	m_vecMaterialSlot[Index]->SetScene(m_Scene);
}

void CLandScape::SetMaterial(const std::string& Name, int Index)
{
	CMaterial* Material = m_Scene->GetResource()->FindMaterial(Name);

	m_vecMaterialSlot[Index] = Material->Clone();

	m_vecMaterialSlot[Index]->SetScene(m_Scene);
}

void CLandScape::AddMaterial(CMaterial* Material)
{
	m_vecMaterialSlot.push_back(Material->Clone());

	m_vecMaterialSlot[m_vecMaterialSlot.size() - 1]->SetScene(m_Scene);
}

void CLandScape::AddMaterial(const std::string& Name)
{
	CMaterial* Material = m_Scene->GetResource()->FindMaterial(Name);

	m_vecMaterialSlot.push_back(Material->Clone());

	m_vecMaterialSlot[m_vecMaterialSlot.size() - 1]->SetScene(m_Scene);
}
void CLandScape::SetDetailLevel(float Level)
{
}

void CLandScape::SetSplatCount(int Count)
{
}

float CLandScape::GetHeight(const Vector3& Pos)
{
	Vector3 Convert = (Pos - GetWorldPos()) / GetWorldScale();

	// Z 좌표 역을 계산
	// LandScape 은 좌표가 왼쪽 상단부터 x,z가 0으로 시작하기 때문이다.
	Convert.z = m_CountZ - 1 - Convert.z;

	int IndexX = (int)Convert.x;
	int IndexZ = (int)Convert.z;

	// 왜 X는 - 1 을 안해주고, Z만 해주는 것일까 ?
	if (IndexX < 0 || IndexX >= m_CountX || IndexZ < 0 || IndexZ >= m_CountZ - 1)
		return Pos.y;

	int Index = IndexZ * m_CountX + IndexX;

	float RatioX = Convert.x - IndexX;
	float RatioZ = Convert.z - IndexZ;

	// 왼상, 오상, 왼하, 오하
	float Y[4] =
	{
		m_vecPos[Index].y,
		m_vecPos[Index + 1].y,
		m_vecPos[Index + m_CountX].y,
		m_vecPos[Index + m_CountX + 1].y,
	};

	// 우상단 삼각형
	if (RatioX > RatioZ)
		return Y[0] + (Y[1] - Y[0]) * RatioX + (Y[3] - Y[1])

		return Y[0] + (Y[1] - Y[0] * RatioX) + (Y[2] - Y[0]);
}

void CLandScape::Start()
{
}

bool CLandScape::Init()
{
	m_CBuffer = new CLandScapeConstantBuffer;

	m_CBuffer->Init();

	return true;
}

void CLandScape::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CLandScape::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

void CLandScape::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CLandScape::Render()
{
	CSceneComponent::Render();

	if (!m_Mesh)
		return;

	m_CBuffer->UpdateCBuffer();

	size_t	Size = m_vecMaterialSlot.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecMaterialSlot[i]->Render();

		m_Mesh->Render((int)i);

		m_vecMaterialSlot[i]->Reset();
	}
}

void CLandScape::PostRender()
{
	CSceneComponent::PostRender();
}

CLandScape* CLandScape::Clone()
{
	return new CLandScape(*this);
}

void CLandScape::Save(FILE* File)
{
	CSceneComponent::Save(File);
}

void CLandScape::Load(FILE* File)
{
	CSceneComponent::Load(File);
}

void CLandScape::ComputeNormal()
{
	// 각 정점이 구성하는 면들의 Normal 들의 평균을 내면
	// 해당 정점의 Normal 이 된다.
	size_t Size = m_vecFaceNormal.size();

	for (size_t i = 0; i < Size; ++i)
	{
		unsigned int Idx0 = m_vecIndex[i * 3];
		unsigned int Idx1 = m_vecIndex[i * 3 + 1];
		unsigned int Idx2 = m_vecIndex[i * 3 + 2];

		// i 번째 Face (면)을 구성하는 3개의 정점 정보를 가져와서 해당 정보를 세팅한다.
		m_vecVtx[Idx0].Normal += m_vecFaceNormal[i];
		m_vecVtx[Idx1].Normal += m_vecFaceNormal[i];
		m_vecVtx[Idx2].Normal += m_vecFaceNormal[i];
	}

	Size = m_vecVtx.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecVtx[i].Normal.Normalize();
	}
}

void CLandScape::ComputeTangent()
{
	// Tangent Vector 구현
	for (size_t i = 0; i < m_vecFaceNormal.size(); ++i)
	{
		int idx0 = m_vecIndex[i * 3];
		int idx1 = m_vecIndex[i * 3 + 1];
		int idx2 = m_vecIndex[i * 3 + 2];

		float fVtx1[3], fVtx2[3];
		fVtx1[0] = m_vecVtx[idx1].Pos.x - m_vecVtx[idx0].Pos.x; // e1.x
		fVtx1[1] = m_vecVtx[idx1].Pos.y - m_vecVtx[idx0].Pos.y; // e1.y
		fVtx1[2] = m_vecVtx[idx1].Pos.z - m_vecVtx[idx0].Pos.z; // e1.x

		fVtx2[0] = m_vecVtx[idx2].Pos.x - m_vecVtx[idx0].Pos.x; // e2.x
		fVtx2[1] = m_vecVtx[idx2].Pos.y - m_vecVtx[idx0].Pos.y; // e2.y
		fVtx2[2] = m_vecVtx[idx2].Pos.z - m_vecVtx[idx0].Pos.z; // e2.z

		float	ftu[2], ftv[2];
		ftu[0] = m_vecVtx[idx1].UV.x - m_vecVtx[idx0].UV.x; // x1
		ftv[0] = m_vecVtx[idx1].UV.y - m_vecVtx[idx0].UV.y; // y1

		ftu[1] = m_vecVtx[idx2].UV.x - m_vecVtx[idx0].UV.x; // x2
		ftv[1] = m_vecVtx[idx2].UV.y - m_vecVtx[idx0].UV.y; // y2

		float	fDen = 1.f / (ftu[0] * ftv[1] - ftu[1] * ftv[0]);

		Vector3	Tangent;
		Tangent.x = (ftv[1] * fVtx1[0] - ftv[0] * fVtx2[0]) * fDen;
		Tangent.y = (ftv[1] * fVtx1[1] - ftv[0] * fVtx2[1]) * fDen;
		Tangent.z = (ftv[1] * fVtx1[2] - ftv[0] * fVtx2[2]) * fDen;

		Tangent.Normalize();

		m_vecVtx[idx0].Tangent = Tangent;
		m_vecVtx[idx1].Tangent = Tangent;
		m_vecVtx[idx2].Tangent = Tangent;

		m_vecVtx[idx0].Binormal = m_vecVtx[idx0].Normal.Cross(Tangent);
		m_vecVtx[idx1].Binormal = m_vecVtx[idx1].Normal.Cross(Tangent);
		m_vecVtx[idx2].Binormal = m_vecVtx[idx2].Normal.Cross(Tangent);

		m_vecVtx[idx0].Binormal.Normalize();
		m_vecVtx[idx1].Binormal.Normalize();
		m_vecVtx[idx2].Binormal.Normalize();
	}
}

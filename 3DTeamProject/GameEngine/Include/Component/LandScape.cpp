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

// ������ Height Map �� ����Ͽ�
// ���� ������ŭ�� ������ ���� 
// Mesh �� ������ִ� �Լ�
// �� ������ ��ġ, Normal, Index, Tagnent, Binormal ������ ��� ������ش�.
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

		// �� �ȼ� ���� �����´�. �ȼ� �ϳ��ϳ��� ������ �̾ƿ;� �Ѵ�.
		const Image* PixelInfo = Img.GetImages();

		for (size_t i = 0; i < PixelInfo->height; ++i)
		{
			for (size_t j = 0; j < PixelInfo->width; ++j)
			{
				// �ȼ��� 1byte 4��¥���� �𿩼� �ϳ��� ������ �����.
				// ���� Idx * 4 �� ����� �Ѵ�.
				int PixelIndex = (int)i * (int)PixelInfo->width * 4 + (int)j * 4;

				// �̷��� �ϸ�, r,g,b,a �߿��� r ���� �̾ƿ� ���̴�.
				// Height Map �� Gray Scale ���·� �̾ƿ� ���̴�.
				// ���� r,g,b,a �� � ���� �̾ƿ͵� �������.
				// �ٽ� ���ϸ�,���ʿ� Height Map ��ü��, Gray Scale �̰�
				// �ش� Pixel �� r,g,b,a ����, ���� ���� �������� r,g,b,a �� �����ϰ� ������ �����̹Ƿ�
				// GrayScale ���¸� �� �� �ۿ� ���ٴ� ���̴�.

				// �׸��� GrayScale �� ����� �ȼ��� 0 ���� 255������ ���̴� (r,g,b,a ��ü�� 0���� 255 ������ ��)
				// �׷���, ���� Render �ÿ� ���� 255 �� �ʹ� ū���̴�.
				// ����, 60���� ������ ���̴� ( ������� �����ص� �Ǵ� ��)
				float Y = PixelInfo->pixels[PixelIndex] / 60.f;

				vecY.push_back(Y);
			}
		}
	}
	else
	{
		// ���� ���� ���ٸ�, 0���� �� ��ƹ��� ���̴�.
		vecY.resize(m_CountX * m_CountZ);
	}

	// ���� ���� �����
	for (int i = 0; i < m_CountZ; ++i)
	{
		for (int j = 0; j < m_CountX; ++j)
		{
			// ��� ������ ��ȸ�ϸ鼭, �ش� ������ŭ ������ ���� ���̴�.
			Vertex3D Vtx = {};

			// x,y,z
			// ���� ���������� �������Ƿ� i �� �ƴ϶� m_CountZ - i 
			// ex) 1 ~ 100 --> 0 ~ 99 �� ����� ���� --> ���� - 1
			Vtx.Pos = Vector3((float)j, vecY[i * m_CountX + j], (float)m_CountZ - i - 1);
			// UV �� ���, ex) 0 ~ 99 �� x�� �� �� --> 99�� �� 1 �̶�� ���� ���;� �Ѵ�.
			Vtx.UV = Vector2(j / (float)(m_CountX - 1), i / (float)(m_CountZ - 1));

			m_vecVtx.push_back(Vtx);
		}
	}

	// �ε��� ���� �����
	Vector3 TrianglePos[3];
	Vector3 Edge1, Edge2;
	unsigned int TriIndex = 0;

	// �ﰢ�� ������ŭ ����� 
	// �� - 1�� ���༭ �����ִ� ���ΰ� ?
	// ex) 5*5 ���� --> ���� ��������� �簢���� 4*4 -> 16��
	// ex) N���� �簢�� --> N * 2���� �ﰢ�� �� ��ȸ�ϸ鼭 ���� ���� ������ �������� �Ѵ�.
	m_vecFaceNormal.resize((m_CountX - 1)* (m_CountZ - 1) * 2);

	for (unsigned int i = 0; i < m_CountZ - 1; ++i)
	{
		for (unsigned int j = 0; j < m_CountX - 1; ++j)
		{
			unsigned int Index = i * m_CountX + j;

			// �簢���� ���� �ﰢ�� (�ð���� -> ���� �ﰢ�� ���·� ����� ���Ͽ�)
			m_vecIndex.push_back(Index);
			m_vecIndex.push_back(Index + 1);
			m_vecIndex.push_back(Index + m_CountX + 1);

			TrianglePos[0] = m_vecVtx[Index].Pos;
			TrianglePos[1] = m_vecVtx[Index + 1].Pos;
			TrianglePos[2] = m_vecVtx[Index + m_CountX + 1].Pos;

			// ������ ��Ģ --> �츮������ ���ϴ� ���� ���͸� ������
			Edge1 = TrianglePos[1] - TrianglePos[0];
			Edge2 = TrianglePos[2] - TrianglePos[0];

			Edge1.Normalize();
			Edge2.Normalize();

			m_vecFaceNormal[TriIndex] = Edge1.Cross(Edge2);
			m_vecFaceNormal[TriIndex].Normalize();

			++TriIndex;

			// �簢���� ���ϴ� �ﰢ��
			m_vecIndex.push_back(Index);
			m_vecIndex.push_back(Index + m_CountX + 1);
			m_vecIndex.push_back(Index + m_CountX);

			TrianglePos[0] = m_vecVtx[Index].Pos;
			TrianglePos[1] = m_vecVtx[Index + m_CountX + 1].Pos;
			TrianglePos[2] = m_vecVtx[Index + m_CountX].Pos;

			// ������ ��Ģ --> �츮������ ���ϴ� ���� ���͸� ������
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

	// �޽��� �����.
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

	// Z ��ǥ ���� ���
	// LandScape �� ��ǥ�� ���� ��ܺ��� x,z�� 0���� �����ϱ� �����̴�.
	Convert.z = m_CountZ - 1 - Convert.z;

	int IndexX = (int)Convert.x;
	int IndexZ = (int)Convert.z;

	// �� X�� - 1 �� �����ְ�, Z�� ���ִ� ���ϱ� ?
	if (IndexX < 0 || IndexX >= m_CountX || IndexZ < 0 || IndexZ >= m_CountZ - 1)
		return Pos.y;

	int Index = IndexZ * m_CountX + IndexX;

	float RatioX = Convert.x - IndexX;
	float RatioZ = Convert.z - IndexZ;

	// �޻�, ����, ����, ����
	float Y[4] =
	{
		m_vecPos[Index].y,
		m_vecPos[Index + 1].y,
		m_vecPos[Index + m_CountX].y,
		m_vecPos[Index + m_CountX + 1].y,
	};

	// ���� �ﰢ��
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
	// �� ������ �����ϴ� ����� Normal ���� ����� ����
	// �ش� ������ Normal �� �ȴ�.
	size_t Size = m_vecFaceNormal.size();

	for (size_t i = 0; i < Size; ++i)
	{
		unsigned int Idx0 = m_vecIndex[i * 3];
		unsigned int Idx1 = m_vecIndex[i * 3 + 1];
		unsigned int Idx2 = m_vecIndex[i * 3 + 2];

		// i ��° Face (��)�� �����ϴ� 3���� ���� ������ �����ͼ� �ش� ������ �����Ѵ�.
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
	// Tangent Vector ����
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

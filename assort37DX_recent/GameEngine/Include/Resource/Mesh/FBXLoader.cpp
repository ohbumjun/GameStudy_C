
#include "FBXLoader.h"

CFBXLoader::CFBXLoader() :
	m_Manager(nullptr),
	m_Scene(nullptr)
{
}

CFBXLoader::~CFBXLoader()
{
	size_t	Size = m_vecMaterials.size();

	for (size_t i = 0; i < Size; ++i)
	{
		size_t	Size1 = m_vecMaterials[i].size();

		for (size_t j = 0; j < Size1; ++j)
		{
			SAFE_DELETE(m_vecMaterials[i][j]);
		}
	}

	m_vecMaterials.clear();

	Size = m_vecMeshContainer.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecMeshContainer[i]);
	}

	m_vecMeshContainer.clear();

	for (int i = 0; i < m_NameArr.Size(); ++i)
	{
		m_NameArr[i]->Clear();
		SAFE_DELETE(m_NameArr[i]);
	}

	m_NameArr.Clear();

	Size = m_vecBones.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecBones[i]);
	}

	m_vecBones.clear();

	Size = m_vecClip.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecClip[i]);
	}

	m_vecClip.clear();

	m_Scene->Clear();
	m_Scene->Destroy();
	m_Manager->Destroy();
}

bool CFBXLoader::LoadFBX(const char* FullPath, bool Static)
{
	// FbxManager 객체를 생성한다.
	m_Manager = FbxManager::Create();

	// IOSetting을 생성한다.
	// ex) 파일의 내용을 읽을 것인가, 아니면, FBX 파일을 쓸 것인가 (만들 것인가) 에 대한 정보를 담고 있다.
	FbxIOSettings* Ios = FbxIOSettings::Create(m_Manager, IOSROOT);

	// FbxManager에 지정한다.
	m_Manager->SetIOSettings(Ios);

	// FbxScene을 생성한다.
	m_Scene = FbxScene::Create(m_Manager, "");

	// FbxImporter 를 이용해서 fbx 파일로부터 메쉬 정보를 읽어온다.
	// ex) 만약 FBX 파일을 만들어내고 싶다면, Exporter 라는 객체 활용
	FbxImporter* Importer = FbxImporter::Create(m_Manager, "");

	// 해당 경로에 있는 fbx 파일을 읽어오고 정보를 만들어낸다.
	Importer->Initialize(FullPath, -1, m_Manager->GetIOSettings());

	// 위에서 만들어낸 정보를 FbxScene에 노드를 구성한다.
	// 즉, Scene Graph (트리) 를 만들면서, 읽은 내용을 해당 Tree 에 쫘라락 넣어준다.
	Importer->Import(m_Scene);

	if (m_Scene->GetGlobalSettings().GetAxisSystem() != FbxAxisSystem::Max)
		m_Scene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::Max);

	// Static Type 이 아니라는 것은, Animation 이 있는 Mesh 라는 것이다.
	if (!Static)
	{
		// 현재 존재하는 Animation Stack 이름으로, Array 정보를 채워넣는다.
		m_Scene->FillAnimStackNameArray(m_NameArr);

		// 개수가 0 이상이라면, Animation이 존재한다는 의미이다.
		if (m_NameArr.GetCount() > 0)
		{
			LoadAnimationClip();

			// 본 정보를 읽어온다.
			LoadBone(m_Scene->GetRootNode());

			// 클립이 가지고 있는 프레임을 본 개수만큼 resize 해준다.
			// 원래 Animation Clip에서 하던것이다.
			for (size_t i = 0; i < m_vecClip.size(); ++i)
			{
				m_vecClip[i]->vecBoneKeyFrame.resize(m_vecBones.size());
			}
		}
	}

	// 아래 2개의 함수에 Root Node 를 넘겨주면 , 모든 Tree 를 순회하면서, 재귀적으로 해당 함수들을 호출할 것이다.
	// 해당 함수는, 각 노드들을 순회하면서 FBX 의 재질 정보들을 가져오는 함수이다.
	Triangulate(m_Scene->GetRootNode());

	// 위에서, 재질 정보를 가져왔다면, Mesh 를 구성하는 정점 정보.도 만들어야 한다.
	LoadMesh(m_Scene->GetRootNode(), Static);

	// Importer 를 Destroy 시켜준다.
	Importer->Destroy();

	return true;
}

void CFBXLoader::Triangulate(FbxNode* pNode)
{
	// FbxNodeAttribute : FbxNode 속성을 나타내는 인터페이스이다.
	// 노드로부터 얻어올 수 있다.
	FbxNodeAttribute* pAttr = pNode->GetNodeAttribute();

	// 메쉬 정보를 가지고 있는 노드일 경우
	if (pAttr && (pAttr->GetAttributeType() == FbxNodeAttribute::eMesh ||
		pAttr->GetAttributeType() == FbxNodeAttribute::eNurbs ||
		pAttr->GetAttributeType() == FbxNodeAttribute::eNurbsSurface))
	{
		FbxGeometryConverter	converter(m_Manager);

		// pReplace 의 bool 변수를 통해서, 들고 있는 Mesh 정보를 바꿀지 말지를 결정할 수 있다.
		// ex) 애니메이션 관련된 녀석이 있다면, 거기에 맞춰서 정보를 바꾸는 원리이다.
		converter.Triangulate(pAttr, true);
	}

	// 재질정보를 읽는다.
	int	iMtrlCount = pNode->GetMaterialCount();

	// 해당 노드가 재질 정보를 가지고 있다는 의미이다.
	if (iMtrlCount > 0)
	{
		std::vector<PFBXMATERIAL>	vecMtrl;

		m_vecMaterials.push_back(vecMtrl);

		for (int i = 0; i < iMtrlCount; ++i)
		{
			// FbxSurfaceMaterial : FbxNode 가 가지고 있는 재질 정보를
			// 담고있다.
			FbxSurfaceMaterial* pMtrl = pNode->GetMaterial(i);

			if (!pMtrl)
				continue;

			LoadMaterial(pMtrl);
		}
	}

	// 현재 노드의 자식노드 수를 얻어온다.
	int	iChildCount = pNode->GetChildCount();

	for (int i = 0; i < iChildCount; ++i)
	{
		Triangulate(pNode->GetChild(i));
	}
}

void CFBXLoader::LoadMaterial(FbxSurfaceMaterial* pMtrl)
{
	// 재질 정보를 저장할 구조체를 생성한다. (추가할 재질 정보 동적할당 )
	PFBXMATERIAL	pMtrlInfo = new FBXMATERIAL;

	// 가장 마지막 컨테이너에 재질 정보를 추가한다.
	m_vecMaterials[m_vecMaterials.size() - 1].push_back(pMtrlInfo);

	// Diffuse 정보를 읽어온다.
	pMtrlInfo->BaseColor = GetMaterialColor(pMtrl, FbxSurfaceMaterial::sDiffuse,
		FbxSurfaceMaterial::sDiffuseFactor);

	pMtrlInfo->Amb = GetMaterialColor(pMtrl, FbxSurfaceMaterial::sAmbient,
		FbxSurfaceMaterial::sAmbientFactor);

	pMtrlInfo->Spc = GetMaterialColor(pMtrl, FbxSurfaceMaterial::sSpecular,
		FbxSurfaceMaterial::sSpecularFactor);

	pMtrlInfo->Emv = GetMaterialColor(pMtrl, FbxSurfaceMaterial::sEmissive,
		FbxSurfaceMaterial::sEmissiveFactor);

	pMtrlInfo->SpecularPower = (float)GetMaterialFactor(pMtrl, FbxSurfaceMaterial::sSpecularFactor);
	pMtrlInfo->Shininess = (float)GetMaterialFactor(pMtrl, FbxSurfaceMaterial::sShininess);
	pMtrlInfo->TransparencyFactor = (float)GetMaterialFactor(pMtrl, FbxSurfaceMaterial::sTransparencyFactor);

	// FBX 파일에는, Texture 정보도 담을 수 있다. 따라서 해당 Texture 파일을 읽어오면
	// 새로운 Texture 파일을 만들어주기도 한다.
	// 해당 Texture 파일 들의 "경로" 정보를 return 해준다. (std::string 형태)ㄴ
	pMtrlInfo->DifTex = GetMaterialTexture(pMtrl, FbxSurfaceMaterial::sDiffuse);
	pMtrlInfo->BumpTex = GetMaterialTexture(pMtrl, FbxSurfaceMaterial::sNormalMap);

	if (pMtrlInfo->BumpTex.empty())
		pMtrlInfo->BumpTex = GetMaterialTexture(pMtrl, FbxSurfaceMaterial::sBump);

	pMtrlInfo->SpcTex = GetMaterialTexture(pMtrl, FbxSurfaceMaterial::sSpecular);
}

Vector4 CFBXLoader::GetMaterialColor(FbxSurfaceMaterial* pMtrl,
	const char* pPropertyName, const char* pPropertyFactorName)
{
	FbxDouble3	vResult(0, 0, 0);
	double		dFactor = 0;

	// 인자로 들어온 재질에서 해당 이름을 가진 프로퍼티가 있는지를 판단한다음
	// 얻어온다. 마찬가지로 해당 이름의 프로퍼티 팩터가 있는지를 판단한다음
	// 얻어온다.
	FbxProperty	tProperty = pMtrl->FindProperty(pPropertyName);
	FbxProperty	tPropertyFactor = pMtrl->FindProperty(pPropertyFactorName);

	// 유효한지 체크한다.
	if (tProperty.IsValid() && tPropertyFactor.IsValid())
	{
		// Property 에는 FbxDouble3 형태로 색상 정보가 들어있다.
		vResult = tProperty.Get<FbxDouble3>();

		// 색상의 비율을 구하는 함수
		dFactor = tPropertyFactor.Get<FbxDouble>();

		// Factor가 1이 아닐 경우 일종의 비율값으로 사용하여
		// 얻어온 vResult 에 모두 곱해준다.
		if (dFactor != 1)
		{
			vResult[0] *= dFactor;
			vResult[1] *= dFactor;
			vResult[2] *= dFactor;
		}
	}

	return Vector4((float)vResult[0], (float)vResult[1], (float)vResult[2], (float)dFactor);
}

double CFBXLoader::GetMaterialFactor(FbxSurfaceMaterial* pMtrl,
	const char* pPropertyName)
{
	FbxProperty	tProperty = pMtrl->FindProperty(pPropertyName);
	double	dFactor = 0.0;

	if (tProperty.IsValid())
		dFactor = tProperty.Get<FbxDouble>();

	return dFactor;
}

std::string CFBXLoader::GetMaterialTexture(FbxSurfaceMaterial* pMtrl,
	const char* pPropertyName)
{
	FbxProperty	tProperty = pMtrl->FindProperty(pPropertyName);

	std::string	str = "";

	if (tProperty.IsValid())
	{
		// FbxFileTexture 의 타입이 몇개가 있는지를 얻어오는 함수이다.
		int	iTexCount = tProperty.GetSrcObjectCount<FbxFileTexture>();

		if (iTexCount > 0)
		{
			FbxFileTexture* pFileTex = tProperty.GetSrcObject<FbxFileTexture>(0);

			if (pFileTex)
				str = pFileTex->GetFileName();
		}
	}

	return str;
}

void CFBXLoader::LoadMesh(FbxNode* pNode, bool bStatic)
{
	FbxNodeAttribute* pAttr = pNode->GetNodeAttribute();

	// Mesh 를 들고 있는 노드라면, Load Mesh 함수를 호출한다.
	if (pAttr && pAttr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		FbxMesh* pMesh = pNode->GetMesh();

		// 실질적으로 Mesh 정보를 Load 하는 함수이다.
		if (pMesh)
			LoadMesh(pMesh, bStatic);
	}

	// 자식 노드들 순회
	int	iChildCount = pNode->GetChildCount();

	for (int i = 0; i < iChildCount; ++i)
	{
		LoadMesh(pNode->GetChild(i), bStatic);
	}
}

void CFBXLoader::LoadMesh(FbxMesh* pMesh, bool bStatic)
{
	// MeshContainer를 만들고 컨테이너별 정점과 컨테이너&서브셋 별
	// 인덱스 정보를 얻어와서 메쉬를 구성해야 한다.
	PFBXMESHCONTAINER	pContainer = new FBXMESHCONTAINER;

	// 서브셋 이란 ?
	// ex) 팔 컨테이너 라고 한다면 --> 주먹, 이두 .. 이런 식으로 각각의 Subset 이 존재하는 것이다.
	m_vecMeshContainer.push_back(pContainer);

	// ControlPoint 는 위치정보를 담고 있는 배열이다.
	// 이 배열의 개수는 곧 정점의 개수가 된다.
	// ex) 즉, ControlPoint 는 물리적인 정점 --> 특정 Object 의 8개 정점 == FBX 파일에서 8개의 Control Point
	int	iVtxCount = pMesh->GetControlPointsCount();

	// 내부적으로 FbxVector4타입의 배열로 저장하고 있기 때문에 배열의 
	// 시작주소를 얻어온다.
	FbxVector4* pVtxPos = pMesh->GetControlPoints();

	// 컨테이너가 가지고 있는 정점 정보들을 정점수만큼 resize 해준다.
	pContainer->vecPos.resize(iVtxCount);
	pContainer->vecNormal.resize(iVtxCount);
	pContainer->vecUV.resize(iVtxCount);
	pContainer->vecTangent.resize(iVtxCount);
	pContainer->vecBinormal.resize(iVtxCount);

	// 보는 바와 같이 vecPos 의 경우, 나머지 성분 들과 다소 다르게 처리함을 볼 수 있다
	// 각 정점들은, 정점 하나당 하나의 Pos 정보를 지닌다.
	// 반면, Normal 의 경우, 하나의 정점이라도, 해당 정점이 속한 Polygon 에 따라서, 하나의 정점이 여러개의 Normal Vector 를 지닐 수도 있다.
	for (int i = 0; i < iVtxCount; ++i)
	{
		// y와 z축이 바뀌어 있기 때문에 변경해준다.
		// 원래 x,y,z 순으로 넣어야 하는데. 좌표계가 다르기 때문에 이와 같이 세팅한다.
		// 즉, 왼손좌표계 (DX) -- 전방이 Z, 위가 Y
		// 3dMax -- Z 가 위, 전방이 Y
		// Y,Z 를 바꿔주기 위해 이와 같이 세팅하는 것이다.
		pContainer->vecPos[i].x = (float)pVtxPos[i].mData[0];
		pContainer->vecPos[i].y = (float)pVtxPos[i].mData[2];
		pContainer->vecPos[i].z = (float)pVtxPos[i].mData[1];
	}

	// 폴리곤 수를 얻어온다.
	// (삼각형 개수)
	int	iPolygonCount = pMesh->GetPolygonCount();

	UINT	iVtxID = 0;

	// 재질 수를 얻어온다.(재질 수 == 서브셋 수)
	int	iMtrlCount = pMesh->GetNode()->GetMaterialCount();

	// 만약, 재질이 없다면, 1개를 만들어준다.
	// 즉, Subset 하나에 최소 1개의 재질을 가지고 있게 하는 것이다.
	iMtrlCount = iMtrlCount == 0 ? 1 : iMtrlCount;

	// 재질 수는 곧 서브셋 수이기 때문에 재질 수만큼 resize 한다.
	// 각 Subset 마다, Idx 버퍼 정보도 다를 것이다.
	pContainer->vecIndices.resize(iMtrlCount);

	// 재질 정보를 얻어온다.
	FbxGeometryElementMaterial* pMaterial = pMesh->GetElementMaterial();

	int iCount = pMesh->GetElementMaterialCount();

	// 삼각형 수만큼 반복한다.
	for (int i = 0; i < iPolygonCount; ++i)
	{
		// 이 폴리곤을 구성하는 정점의 수를 얻어온다.
		// 삼각형일 경우 3개를 얻어온다.
		int	iPolygonSize = pMesh->GetPolygonSize(i);

		// 3개 짜리를 주로 작업할 것이다.
		// ex) 삼각형 내 각 정점의 // 해당 삼각형에서의 Idx
		int	iIdx[3] = {};

		for (int j = 0; j < iPolygonSize; ++j)
		{
			// 현재 삼각형을 구성하고 있는 버텍스정보 내에서의 인덱스를
			// 구한다.
			int 	iControlIndex = pMesh->GetPolygonVertex(i, j);

			// Idx 정보 저장
			iIdx[j] = iControlIndex;

			// ex) iControlIndex => 해당 삼각형에 속한, 해당 정점에서의, Normal 정보
			// ex) 3번째 삼각형 2번째 정점 --> 3 * 2 + 2  = 8 ? (iControlIndex)
			LoadNormal(pMesh, pContainer, iVtxID, iControlIndex);

			// ex) 3번째 삼각형 2번째 정점의 Idx
			LoadUV(pMesh, pContainer, pMesh->GetTextureUVIndex(i, j), iControlIndex);

			LoadTangent(pMesh, pContainer, iVtxID, iControlIndex);

			LoadBinormal(pMesh, pContainer, iVtxID, iControlIndex);

			// 하나하나가 정점 정보이므로, 처리할 때마다 정점 개수 ++ 을 해준다.
			++iVtxID;
		}

		// 삼각형이 소속된 서브셋 인덱스를 얻어온다.
		int	iMtrlID = 0;

		if (pMaterial)
			iMtrlID = pMaterial->GetIndexArray().GetAt(i);

		// 인덱스 버퍼 ? 정보를 세팅해준다.
		// ex) 팔 컨테이너 --> 주먹 Subset 에서 사용되는 인덱스 버퍼 정보 , 이두 Subset 에서 사용되는 인덱스 버퍼 정보
		// DX 기준으로 Y,Z 를 바꿔주기 위해 0, 2, 1 순으로 넣어준다.
		pContainer->vecIndices[iMtrlID].push_back(iIdx[0]);
		pContainer->vecIndices[iMtrlID].push_back(iIdx[2]);
		pContainer->vecIndices[iMtrlID].push_back(iIdx[1]);
	}

	if (!bStatic)
		LoadAnimation(pMesh, pContainer);
}

void CFBXLoader::LoadNormal(FbxMesh* pMesh, PFBXMESHCONTAINER pContainer,
	int iVtxID, int iControlIndex)
{
	// 메쉬로부터 ElementNormal 정보를 얻어온다.
	FbxGeometryElementNormal* pNormal = pMesh->GetElementNormal();

	int 	iNormalIndex = iVtxID;

	// MappingMode와 ReferenceMode에 따라서 인덱스로 사용할 정보가 달라진다.
	// Mode --> FBX 개요 참고
	if (pNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (pNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
			iNormalIndex = pNormal->GetIndexArray().GetAt(iVtxID);
			break;
		}
	}

	else if (pNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		// 안에서 또 다시 Reference Mode 라는 것에 따라 분류하고 있다. FBX 에서 사용하는 최적화의 일종이다.
		// ex) 컴퓨터 그래픽스에서 인덱스 버퍼와 비슷한 개념으로, Vector3  벡터의 중복을 피하기 위해 인덱스를 통해 참조하는 것도 가능하다.
		switch (pNormal->GetReferenceMode())
		{
			// control point 의 인덱스, 혹은 각 면의 정점의 인덱스. 를 통해 Normal 에 접근하는 것
		case FbxGeometryElement::eDirect:
			iNormalIndex = iControlIndex;
			break;

			// control point의 인덱스를 사용하거나 각 면의 정점의 인덱스를 통해
			// normal 벡터를 가리킬 수 있는 인덱스를 부여받고 (한 정점에 대해서도, 해당 정점이 속한 Polygon 개수만큼의
			//	Normal 벡터를 가질 수 있고, 이로 인해, 한 정점이 여러개의 Normal 벡터 인덱스를 가질 수 있다.), 이것을 통해 실제 normal에 접근할 수 있다.
			// 즉, 한번 더 거쳐서 접근하는 개념
		case FbxGeometryElement::eIndexToDirect:
			iNormalIndex = pNormal->GetIndexArray().GetAt(iControlIndex);
			break;
		}
	}

	// 실제 Normal 정보를 가져온다.
	FbxVector4	 vNormal = pNormal->GetDirectArray().GetAt(iNormalIndex);

	// 위치에 맞게 0, 2, 1, 순서로 Normal 세팅하기 (좌표계를 DX 에 맞춰주기 위해서이다.)
	// iControlIndex -> 정점 Idx 라고 생각하면 된다.
	pContainer->vecNormal[iControlIndex].x = (float)vNormal.mData[0];
	pContainer->vecNormal[iControlIndex].y = (float)vNormal.mData[2];
	pContainer->vecNormal[iControlIndex].z = (float)vNormal.mData[1];
}

void CFBXLoader::LoadUV(FbxMesh* pMesh, PFBXMESHCONTAINER pContainer,
	int iUVID, int iControlIndex)
{
	int iCount = pMesh->GetElementUVCount();

	// 특정 Mesh 여러개의 UV 를 들고 있을 수 있다. 이 중에서 첫번째 를 가져온다.
	FbxGeometryElementUV* pUV = pMesh->GetElementUV(0);

	// 만약 첫번째 UV 정보가 존재하지 않는다면, UV 정보 자체가 아예 없다는 것이므로 함수 종료
	if (!pUV)
		return;

	int	iUVIndex = iUVID;

	// eByControlPoint -> 쉽게 말해, 정점 하나 당 하나의 Normal 값
	if (pUV->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (pUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			iUVIndex = iControlIndex; // ex) 3번째 삼각형의 2번째 Idx 정점 => (3 * 3 + 2 = 11) 
			break;
		case FbxGeometryElement::eIndexToDirect: // Idx 정보를 모아둔 Idx Array 상에서, Data 에 접근할 수 있는 Idx 정보를 가져오는 코드 
			iUVIndex = pUV->GetIndexArray().GetAt(iControlIndex);
			break;
		}
	}

	// eByPolygonVertex -> 쉽게 말해, 정점 하나 당 여러개의 Normal 값
	else if (pUV->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (pUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
			break;
		}
	}

	// 실제 UV 정보를 가져온다.
	FbxVector2 	vUV = pUV->GetDirectArray().GetAt(iUVIndex);

	// UV 좌표를 0 에서 1 사이의 값으로 맞춰준다.
	pContainer->vecUV[iControlIndex].x = (float)vUV.mData[0] - (int)vUV.mData[0];
	// DX 에서는 Y 좌표가 위, 아래 반전되어 있기 때문에, DX 에서 다시 반전 시켜서 활용한다.
	pContainer->vecUV[iControlIndex].y = 1.f - (float)(vUV.mData[1] - (int)vUV.mData[1]);
}

void CFBXLoader::LoadTangent(FbxMesh* pMesh,
	PFBXMESHCONTAINER pContainer, int iVtxID, int iControlIndex)
{
	FbxGeometryElementTangent* pTangent = pMesh->GetElementTangent();

	if (!pTangent)
		return;

	// Tangent 사용시 Bump 를 true 로 세팅한다.
	pContainer->Bump = true;

	int	iTangentIndex = iVtxID;

	if (pTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (pTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
			iTangentIndex = pTangent->GetIndexArray().GetAt(iVtxID);
			break;
		}
	}

	else if (pTangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (pTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			iTangentIndex = iControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			iTangentIndex = pTangent->GetIndexArray().GetAt(iControlIndex);
			break;
		}
	}

	FbxVector4	vTangent = pTangent->GetDirectArray().GetAt(iTangentIndex);

	pContainer->vecTangent[iControlIndex].x = (float)vTangent.mData[0];
	pContainer->vecTangent[iControlIndex].y = (float)vTangent.mData[2];
	pContainer->vecTangent[iControlIndex].z = (float)vTangent.mData[1];
}

void CFBXLoader::LoadBinormal(FbxMesh* pMesh,
	PFBXMESHCONTAINER pContainer, int iVtxID, int iControlIndex)
{
	// Binormal 도 LoadTangent 와 구조가 동일
	FbxGeometryElementBinormal* pBinormal = pMesh->GetElementBinormal();

	if (!pBinormal)
		return;

	pContainer->Bump = true;

	int	iBinormalIndex = iVtxID;

	if (pBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (pBinormal->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
			iBinormalIndex = pBinormal->GetIndexArray().GetAt(iVtxID);
			break;
		}
	}

	else if (pBinormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (pBinormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			iBinormalIndex = iControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			iBinormalIndex = pBinormal->GetIndexArray().GetAt(iControlIndex);
			break;
		}
	}

	FbxVector4	vBinormal = pBinormal->GetDirectArray().GetAt(iBinormalIndex);

	pContainer->vecBinormal[iControlIndex].x = (float)vBinormal.mData[0];
	pContainer->vecBinormal[iControlIndex].y = (float)vBinormal.mData[2];
	pContainer->vecBinormal[iControlIndex].z = (float)vBinormal.mData[1];
}

void CFBXLoader::LoadAnimationClip()
{
	int	iCount = m_NameArr.GetCount();

	// Time Mode 를 세팅하기 
	FbxTime::EMode	eTimeMode = m_Scene->GetGlobalSettings().GetTimeMode();

	// 애니메이션 이름 개수만큼 반복을 돌릴 것이다.
	for (int i = 0; i < iCount; ++i)
	{
		// m_NameArr에 저장된 이름으로 Scene으로부터 FbxAnimStack 객체를 얻어온다.
		FbxAnimStack* pAnimStack = m_Scene->FindMember<FbxAnimStack>(m_NameArr[i]->Buffer());

		if (!pAnimStack)
			continue;

		// 하나의 애니메이션 정보를 담는 FBXANIMATIONCLIP Class 를 동적할당한다.
		PFBXANIMATIONCLIP	pClip = new FBXANIMATIONCLIP;

		pClip->strName = pAnimStack->GetName();

		// mixamo 에서 가져온 Animation의 경우, mixamo.com 이라는 이름으로 공통되게 들어온다.
		// 이때는 m_Mixamo 를 사용한다고 표시한다.
		if (pClip->strName == "mixamo.com")
			m_Mixamo = true;

		// Imported File의 정보를 지ㅣㄴ고 있다.
		FbxTakeInfo* pTake = m_Scene->GetTakeInfo(pClip->strName.c_str());

		// 시간 정보를 가져온다.
		pClip->tStart = pTake->mLocalTimeSpan.GetStart();
		pClip->tEnd = pTake->mLocalTimeSpan.GetStop();

		// GetFrameCount 함수를 호출하고  time 모드를 넣어주면 시간을 프레임으로
		// 변환해준다. 몇프레임 짜리 애니메이션 인지를 구해준다.
		pClip->lTimeLength = pClip->tEnd.GetFrameCount(eTimeMode) -
			pClip->tStart.GetFrameCount(eTimeMode);

		pClip->eTimeMode = eTimeMode;

		m_vecClip.push_back(pClip);
	}
}

void CFBXLoader::LoadBone(FbxNode* pNode)
{
	int	iChildCount = pNode->GetChildCount();

	// Bone 의 경우, 트리구조로 되어 있다.
	// 따라서 아래와 같이 세팅하는 것이다.
	for (int i = 0; i < iChildCount; ++i)
	{
		LoadBoneRecursive(pNode->GetChild(i), 0, 0, -1);
	}
}

void CFBXLoader::LoadBoneRecursive(FbxNode* pNode, int iDepth, int iIndex, int iParent)
{
	FbxNodeAttribute* pAttr = pNode->GetNodeAttribute();

	// Skeleton Attribute 를 들고 있는 Node 라면, 이를 바탕으로 FBXBONE Class 를 동적할당한다.
	if (pAttr && pAttr->GetAttributeType() ==
		FbxNodeAttribute::eSkeleton)
	{
		PFBXBONE	pBone = new FBXBONE;

		pBone->strName = pNode->GetName();

		// 앞에 불필요하게 붙은 .mixamo.com 이라는 글자를 지우기 위한 것이다.
		if (m_Mixamo)
			pBone->strName.erase(0, 10);

		pBone->iDepth = iDepth;
		pBone->iParentIndex = iParent;

		m_vecBones.push_back(pBone);
	}

	// 그렇다면 Bone은 어떤 방식으로 세팅하는 것일까 ?
	// ex) 손에 해당하는 Bone 에 칼. Item을 붙여놓으면 되는 식
	// ex) 말 타기의 경우, 엉덩이 아래에 Riding Bone 을 만들어두고, 거기에 Riding Horse 를 세팅한다.

	int	iChildCount = pNode->GetChildCount();

	for (int i = 0; i < iChildCount; ++i)
	{
		LoadBoneRecursive(pNode->GetChild(i), iDepth + 1,
			m_vecBones.size(), iIndex);
	}
}

void CFBXLoader::LoadAnimation(FbxMesh* pMesh, PFBXMESHCONTAINER pContainer)
{
	// FbxSkin 에 저점의 Skinning 정보를 담고 있고, FbxSkin은 FbxDeformer 객체를 상속받고 있다.
	// 아래 함수를 통해, FbxSkin 개수가 나오게 된다.
	int 	iSkinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);

	if (iSkinCount <= 0)
		return;

	// 메쉬의 정점 수를 얻어온다.
	int	iCPCount = pMesh->GetControlPointsCount();

	// 정점의 가중치 정보와 본 인덱스 정보는 "정점 수"만큼
	// 만들어져야 한다.
	pContainer->vecBlendWeight.resize(iCPCount);
	pContainer->vecBlendIndex.resize(iCPCount);
	pContainer->Animation = true;

	// 해당 노드에 대한, 최종 월드 행렬
	FbxAMatrix 	matTransform = GetTransform(pMesh->GetNode());

	// Skinning 개수만큼 반복한다.
	for (int i = 0; i < iSkinCount; ++i)
	{
		// 정점의 Skinning 정보를 담고 있는 FbxSkin 정보를 가져온다.
		FbxSkin* pSkin = (FbxSkin*)pMesh->GetDeformer(i, FbxDeformer::eSkin);

		if (!pSkin)
			continue;

		// Skinning 정보를 가져온다.
		FbxSkin::EType	eSkinningType = pSkin->GetSkinningType();

		if (eSkinningType == FbxSkin::eRigid ||
			eSkinningType == FbxSkin::eLinear ||
			eSkinningType == FbxSkin::eBlend)
		{
			// Cluster : 관절을 의미한다. 
			// ex) Cluster 는, Skinning 정보가 있는 뼈대의 갯수만 준다
			// ex) 뼈대가 70 개이고, 그 중에서 Skinning 정보가 있는 뼈대가 20개만 있다고 하면, GetClusterCount 함수로 나온 값은 20이 된다.
			int 	iClusterCount = pSkin->GetClusterCount();

			// 이 관절 개수만큼 반복을 돌릴 것이다.
			for (int j = 0; j < iClusterCount; ++j)
			{
				// 실제 특정 뼈대, 관절을 얻는다.
				FbxCluster* pCluster = pSkin->GetCluster(j);

				// 클러스터에 대응되는 Node 도 얻을 수 있다. GetLink 함수를 통해서
				if (!pCluster->GetLink())
					continue;

				// 해당 노드의 뼈대 / 관절 이름을 얻을 수 있다.
				std::string	strBoneName = pCluster->GetLink()->GetName();

				if (m_Mixamo)
					strBoneName.erase(0, 10);

				// 관절 ? 뼈대 ? 이름을 통해서, BoneIndex 를 얻어낼 수 있다.
				int 	iBoneIndex = FindBoneFromName(strBoneName);

				LoadWeightAndIndex(pCluster, iBoneIndex, pContainer);

				// Bone 정보는 기본적으로 각각의 Bone Space 에서 정의된 형식으로 들어온다.
				// 따라서, 우리가 사용하는 공간으로 옮겨줘야 한다.
				LoadOffsetMatrix(pCluster, matTransform, iBoneIndex, pContainer);

				// 해당 Bone 의 변환행렬 정보를 세팅해준다.
				m_vecBones[iBoneIndex]->matBone = matTransform;

				LoadTimeTransform(pMesh->GetNode(), pCluster,
					matTransform, iBoneIndex);
			}
		}
	}

	ChangeWeightAndIndices(pContainer);
}

FbxAMatrix CFBXLoader::GetTransform(FbxNode* pNode)
{
	const FbxVector4	vT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4	vR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4	vS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(vT, vR, vS);
}

int CFBXLoader::FindBoneFromName(const std::string& strName)
{
	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		if (m_vecBones[i]->strName == strName)
			return i;
	}

	return -1;
}

void CFBXLoader::LoadWeightAndIndex(FbxCluster* pCluster, int iBoneIndex, PFBXMESHCONTAINER pContainer)
{
	// 각 정점에 대한 Idx Array 내의 원소 개수를 가져온다.
	// pCluster 안에는, Bone 에 대한 정보가 들어있다 .. ? (관절)
	// 그렇다면, 해당 Bone 이 영향을 주는 정점의 개수 ..?
	int	iControlIndicesCount = pCluster->GetControlPointIndicesCount();

	for (int i = 0; i < iControlIndicesCount; ++i)
	{
		FBXWEIGHT	tWeight = {};

		tWeight.iIndex = iBoneIndex;

		// Get the array of control point weights. --> 즉, 해당 Bone 이 i번째 정점에 영향을 주는 
		tWeight.dWeight = pCluster->GetControlPointWeights()[i];

		int 	iClusterIndex = pCluster->GetControlPointIndices()[i];

		// map의 특징 : 키를 이용해 인덱스처럼 접근할 경우 해당 키가 없다면
		// 만들어준다.

		// 그러면, 각 정점 idx 가 key 값이고, 해당 정점에 영향을 미치는 Bone들으 Weight 정보를 vector 형식으로 넣어주는 것인가 ?

		// 선생님 말로는, 해당 map 에 최대 4개만 들어올 수 있다.... 한 정점에 영향을 주는 Bone 은 최대 4개만 처리한다는 의미...?
		pContainer->mapWeights[iClusterIndex].push_back(tWeight);
	}
}

void CFBXLoader::LoadOffsetMatrix(FbxCluster* pCluster,
	const FbxAMatrix& matTransform, int iBoneIndex,
	PFBXMESHCONTAINER pContainer)
{
	// matTransform : 원본 해당 관절 ? 의 World 행렬 정보가 들어가 있다.

	FbxAMatrix	 matCluster;
	FbxAMatrix	 matClusterLink;

	// Cluster 행렬 정보
	pCluster->GetTransformMatrix(matCluster);

	// Cluster 에 걸려 있는 링크노드의 행렬 정보
	pCluster->GetTransformLinkMatrix(matClusterLink);

	FbxVector4	v1 = { 1.0, 0.0, 0.0, 0.0 };
	FbxVector4	v2 = { 0.0, 0.0, 1.0, 0.0 };
	FbxVector4	v3 = { 0.0, 1.0, 0.0, 0.0 };
	FbxVector4	v4 = { 0.0, 0.0, 0.0, 1.0 };

	FbxAMatrix	matReflect;
	matReflect.mData[0] = v1;
	matReflect.mData[1] = v2;
	matReflect.mData[2] = v3;
	matReflect.mData[3] = v4;

	/*
	1 0 0 0   1 2 3 4   1 0 0 0
	0 0 1 0   5 6 7 8   0 0 1 0
	0 1 0 0 * 9 0 1 2 * 0 1 0 0
	0 0 0 1   3 4 5 6   0 0 0 1

	1) 한번 곱하면, 가운데 행 2개가 바뀌는 것을 확인할 수 있다.
	1 0 0 0   1 2 3 4
	0 0 1 0   5 6 7 8
	0 1 0 0 * 9 0 1 2 *
	0 0 0 1   3 4 5 6

	2) 2번째 곱해주면, 열 2개가 바뀌는 것을 확인할 수 있다.
	이러한 과정을 진행해주는 이유는, Y,Z 가 Dx 와 Fbx 가 서로 다르기 때문이다.
	1 2 3 4   1 0 0 0
	9 0 1 2   0 0 1 0
	5 6 7 8 * 0 1 0 0
	3 4 5 6   0 0 0 1

	1 3 2 4
	9 1 0 2
	5 7 6 8
	3 5 4 6
	*/

	FbxAMatrix	matOffset;

	matOffset = matClusterLink.Inverse() * matCluster * matTransform;

	// 결과적으로 offset 행렬이 만들어진다.
	// 그냥 DX 공간으로 만들어주는 FBX 의 공식이라고 할 수 있다.
	matOffset = matReflect * matOffset * matReflect;

	m_vecBones[iBoneIndex]->matOffset = matOffset;
}

void CFBXLoader::LoadTimeTransform(FbxNode* pNode,
	FbxCluster* pCluster, const FbxAMatrix& matTransform,
	int iBoneIndex)
{
	FbxVector4	v1 = { 1.0, 0.0, 0.0, 0.0 };
	FbxVector4	v2 = { 0.0, 0.0, 1.0, 0.0 };
	FbxVector4	v3 = { 0.0, 1.0, 0.0, 0.0 };
	FbxVector4	v4 = { 0.0, 0.0, 0.0, 1.0 };

	FbxAMatrix	matReflect;
	matReflect.mData[0] = v1;
	matReflect.mData[1] = v2;
	matReflect.mData[2] = v3;
	matReflect.mData[3] = v4;

	if (m_Mixamo)
	{
		std::vector<PFBXANIMATIONCLIP>::iterator	iter;
		std::vector<PFBXANIMATIONCLIP>::iterator	iterEnd = m_vecClip.end();

		for (iter = m_vecClip.begin(); iter != iterEnd;)
		{
			// Mixamo 에서 가져온 것에서는, Mixamo 이어야만 제대로 된 것이라고 할 수 있다.
			if ((*iter)->strName != "mixamo.com")
			{
				SAFE_DELETE((*iter));
				iter = m_vecClip.erase(iter);
				iterEnd = m_vecClip.end();
			}

			else
				++iter;
		}
	}

	// m_vecClip -> 애니메이션 배열 
	for (size_t i = 0; i < m_vecClip.size(); ++i)
	{
		// 현재 우리는 Key Frame Animation 을 활용하고 있다.
		// 그러면 2개의 Key Frame Animation 정보를 구하고, 그 사이의 Animation 들은, Key Frame 사이의 보간을 통해서 구해낼 수 있다.
		FbxLongLong	Start = m_vecClip[i]->tStart.GetFrameCount(m_vecClip[i]->eTimeMode);
		FbxLongLong	End = m_vecClip[i]->tEnd.GetFrameCount(m_vecClip[i]->eTimeMode);

		// i번째 Animation --> 그때의 iBoneIndex 번째 Bone의 BoneIndex 를 iBoneIndex로 세팅
		m_vecClip[i]->vecBoneKeyFrame[iBoneIndex].iBoneIndex = iBoneIndex;

		// 전체 프레임 수만큼 반복한다.
		for (FbxLongLong j = Start; j <= End; ++j)
		{
			FbxTime	tTime = {};

			// 현재 프레임에 해당하는 FbxTime을 만들어낸다.
			tTime.SetFrame(j, m_vecClip[i]->eTimeMode);

			// EvaluateGlobalTransform
			FbxAMatrix	matOffset = pNode->EvaluateGlobalTransform(tTime) * matTransform;
			FbxAMatrix	matCur = matOffset.Inverse() * pCluster->GetLink()->EvaluateGlobalTransform(tTime);

			matCur = matReflect * matCur * matReflect;

			FBXKEYFRAME	tKeyFrame = {};

			tKeyFrame.dTime = tTime.GetSecondDouble();
			tKeyFrame.matTransform = matCur;

			// i번째 Animation 의 iBoneIndex 번째 Bone 의 KeyFrame Vector 목록에 현재 만들어낸 tKeyFrame 정보를 넣어준다.
			m_vecClip[i]->vecBoneKeyFrame[iBoneIndex].vecKeyFrame.push_back(tKeyFrame);
		}
	}
}

void CFBXLoader::ChangeWeightAndIndices(PFBXMESHCONTAINER pContainer)
{
	std::unordered_map<int, std::vector<FBXWEIGHT>>::iterator	iter;
	std::unordered_map<int, std::vector<FBXWEIGHT>>::iterator	iterEnd = pContainer->mapWeights.end();

	for (iter = pContainer->mapWeights.begin(); iter != iterEnd; ++iter)
	{
		// 모든 Weight 정보들을 돌면서
		// 4개 이상의 Weight이 있다면, 가중치 값에 따라 내림차순 한 이후
		if (iter->second.size() > 4)
		{
			// 가중치 값에 따라 내림차순 정렬한다.
			sort(iter->second.begin(), iter->second.end(), [](const FBXWEIGHT& lhs, const FBXWEIGHT& rhs)
				{
					return lhs.dWeight > rhs.dWeight;
				});

			double	dSum = 0.0;

			for (int i = 0; i < 4; ++i)
			{
				dSum += iter->second[i].dWeight;
			}

			double	dInterpolate = 1.f - dSum;

			std::vector<FBXWEIGHT>::iterator	iterErase = iter->second.begin() + 4;

			// 최대 4개만 되도록 세팅하고
			iter->second.erase(iterErase, iter->second.end());

			// 앞이 1이 되게 세팅한다.
			iter->second[0].dWeight += dInterpolate;
		}

		Vector4	vWeight;
		Vector4	vIndex;

		for (int i = 0; i < iter->second.size(); ++i)
		{
			vWeight[i] = iter->second[i].dWeight;
			vIndex[i] = iter->second[i].iIndex;
		}

		// 해당 정점의 Weight 정보, 영향을 주는 Bone Idx 정보들을 Vector4 에 저장한다.
		pContainer->vecBlendWeight[iter->first] = vWeight;
		pContainer->vecBlendIndex[iter->first] = vIndex;
	}
}


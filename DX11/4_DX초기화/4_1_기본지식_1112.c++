/*
COM ? ---
- C++ 클래스로 간주하고 사용
- new 키워드로 생성하지 않고, 다 쓰고 난 후에도 delete 이 아니라
Release 라는 method를 호출해주어야 한다
- IUnknown 이라는 COM Interface를 상속받고,
그 안에 Release 라는 메서드가 존재한다.
- 왜냐하면, COM 객체들이 자신만의 고유한 방식으로
메모리를 관리하기 때문이다

교환 사슬과 페이지 전환 ----
전면 버퍼와 후면 버퍼는 하나의 교환사슬(swap chain)을 형성한다.

IDXGISwapChain 이라는 인터페이스로 대표된다.
이 인터페이스는 전면 버퍼 텍스처와
후면 버퍼 텍스쳐를 담으며,

버퍼 크기 변경을 위한 메서드 ResiszeBuffers
버퍼의 제시를 위한 메서드 present 를 제공한다

깊이 버퍼링 ---
- 깊이 버퍼는, 이미지 자료를 담지 않는
텍스처의 한 예이다.(깊이 버퍼도 하나의 텍스처.)

- 깊이 버퍼는 각 픽셀의 깊이 정보를 담는다 0 ~ 1
- 깊이 버퍼의 원속들와 후면 버퍼의 픽셀들은
일대일로 대응된다.

- 깊이 버퍼링 알고리즘은, 다음과 같다
렌더링 되는 각 픽셀의 깊이 값을 계산해서, 깊이 판정을 수행한다.
깊이 판정은 후면 버퍼의 특정 픽셀 위치에 기록될 픽셀들으 깊이들을 비교한다
관찰자에게 가장 가까운 픽셀이 승자가 되어 후면 버퍼에 기록된다.

텍스쳐 자원 뷰 --
- 렌더링 파이프 라인에는 텍스처를 묶을 수 있는 단계들이 여럿 있다.
흔한 용도는, 
1) 텍스처를 렌더 대상으로 묶는 것 (DX가 텍스쳐에 렌더링 하는 경우)
2) 세이더 자원으로서 묶는 것(세이더 안에서 텍스처를 추출하는 경우)

이 2가지 용도의 텍스쳐를 생성할 때에는
텍스처를 묶을 두 파이프라인 단계를 지정한 결속 플래그를 사용한다
D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE

사실 텍스처 자원이 파이프라인의 단계에 직접 묶이는 것은 아니다.
실제로는 텍스처가 연관된 자원 뷰를 파이프라인 단계에 묶는다.

DX에서 텍스처를 사용하려면, 텍스처의 초기화 시점에서
그 텍스처의 자원 뷰를 생성해야 한다(resource view)

따라서, 텍스처를 렌더 대상과 셰이더 자원으로 사용하는 예제를 위해서는
2개의 뷰를 생성할 필요가 있다
1) 렌더 대상 뷰 Render Target View
2) 셰이더 자원 뷰 Shader Resource View
 

*/

// 4.1.8 : Direct3D 의 다중 표본화

typedef struct DXGI_SAMPLE_DESC
{
    // 픽셀당 추출할 표본의 개수 
    UINT Count;
    // 원하는 품질 수준(Quality Level)
    UINT Quality;
} DXGI_SAMPLE_DESC;

// 품질 수준이 높을 수록, 렌더링 비용도 증가한다.
// 멀티 샘플링, 다중 표본화.를 사용하고 싶지 않다면
// 0,1 로 각각 세팅한다.

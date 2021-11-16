/*
1. D3D11CreateDevice 함수를 이용해서
장치, 즉, ID3D11Device 인터페이스와
장치 문맥, 즉, DeviceContext를 생성한다

2. CheckMultisampleQulaityLevels 메서드를 통해
feature level 지원 여부를 점검한다

3. 생성할 교환 사슬의 특성을 서술하는
SWAP_CHAIN_DESC 구조체를 세운다

4. IDXGIFactory 인터페이스를 질의해서
SwapChain 인터페이스를 생성한다

5. 교환 사실의 후면 버퍼에 대한
렌더대상 뷰를 생성한다

6. 깊이, 스탠실 버포와 그에 연결되는
깊이, 스탠실 뷰를 생성한다

7. 렌더 대상 뷰와 같이
깊이,스탠실 뷰를 DX가 사용할 수 있도록
렌더링 파이프라인의 출력 병합기 단계에 묶는다

8. 뷰포트를 설정한다

*/

/*
1. Device, DeviceContext

- 응용 프로그램은, 이 인터페이스들을 통해서
하드웨어에게 할일을 지시한다.

Device        : 기능 지원 점검과 자원 할당
DeviceContext : 렌더 대상을 설정하고, 
자원을 그래픽 파이프라인에 묶고,
GPU가 수행할 렌더링 명령들을 지시하는데 쓰인다 
 

*/

/*
3. Swap Chain
*/
// 생성하고자 하는 후면 버퍼 속성들 
typedef struct DXGI_MODE_DESC
{
    // 원하는후면 버퍼 넓이 
    UNIT Width;
    UINT Height;
    // 디스플레이 모드 갱신율 
    DXGI_RATIONAL RefreshRate;
    DXGI_FORMAT Format; // 후면 버퍼 픽셀 형식
    // 디스플레이 스캔라인 모드 
    DXGI_MODE_SCANLINE_ORDER ScanlineOrdering;
    DXGI_MODE_SCANLING Scaling; // 디스플레이 비례 모드 
};

/*
5. 렌더 대상 뷰의 생성 : Render Target View
어떤 자원을 파이프라인의 단계에 직접 묶는것은 아니다

대신 반드시 자원에 대한 뷰를 생성하고
그 뷰를 파이프라인에 묶어야 한다

후면 버퍼를, 
파이프라인의 출력 병합기(output merger) 단계에 묶으려면
그래야만, 버퍼에 무언가를 그릴 수 있다.

우선, 후면 버퍼에 대한 렌더 대상 뷰를
생성해야 한다.
*/

/*
6. 깊이, 스탠실 버퍼와 뷰 생성

*/
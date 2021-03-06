// 객체를 반드시 초기화 하자 
// 멤버 초기화 리스트를 활용하자 

// 기본제공 타입의 멤버를 초기화 리스트로 넣는 일이
// 선택이 아니라, 의무가 될 때고 있다

// 상수이거나, 참조자로 되어 있는 데이터 멤버의 경우에는
// 반드시 초기화 되어야 한다.

// 왜냐하면, 상수와 참조자는 대입 자체가 불가능하기 때문이다 

// 객체 초기화 순서 ?
// 1. 기본 클래스는 파생 클래스보다 먼저
// 2. 클래스 데이터 멤버는 선언 순서대로 초기화
// 3. 비지역 정적 객체의 초기화 순서는, 개별 번역 단위에서 정해진다

// 정적 객체 static object ?
// 자신이 생성된 시점부터, 프로그램이 끝날 때까지 살아 있는 객체

// - 스택, 힙 기반 객체는 애초부터 정적 객체가 될 수 없다.
// 전역 객체 
// or 네임스페이스 유효범위에서 정의된 객체
// or 클래스 안에서 static
// or 함수 안에서 static
// or 파일 유효 범위에서 static

// 함수 안에 있는 static 은 지역 정적 객체
// 그외 , 비지역 정적 객체 
// 이 모든 정적 객체는, 프로그램이 끝날 때 자동으로 소멸된다.
// 즉, main() 함수의 실행이 끝날 때 정적 객체의 소멸자가 호출된다.

// 번역 단위는, 컴파일을 통해 하나의 목적 파일(object file)을 만드는 
// 바탕이 되는 소스 코드를 일컫습니다.
// 여기서 번역은 소스의 언어를 기계어로 옮긴다는 의미일 것이다
// 기본적으로, 소스 파일 하나가 되는데 , 그 파일이
// #include 하는 파일들까지 합쳐서 하나의 번역 단위가 된다

// 별도로 컴파일된 소스 파일이 2개 이상 있고
// 각 소스 파일에 비지역 정적 객체.가 한개 이상 들어있는 경우
// 한쪽 번역 단위에 있는 비정적 객체의 초기화가 진행되면서
// 다른 쪽 번역 단위에 있는 비지역 정적 객체가 사용되는데
// 객체가 초기화 되어 있지 않을지도 모른다.
// 왜 ?  ---------------------------------------------------
// 별개의 번역 단위에서 정의된 비지역 정적 객체들의 초기화 순서는
// 정해져 있지 않기 때문이다.

class FileSystem // 전역 객체 가정 
{
    public :
        std::size_t numDisks() const ; // 많은 멤버 변수 중 하나 
}
extern FileSystem tfs; // 사용자가 쓰게 될 객체 

class Directory // 다른 위치에서의 전역 객체 가정 
{
public :
    Directory(params);
}

Directory::Directory(params)
{
    std::size_t disks = tfs.numDistks(); // tfs 가 초기화가 안된 상태에서 실행시 컴파일 에러 
}

Directory tempDir(params);


// 이를 해결하는 방법은,
// 비지역 정적 객체를 하나씩 맡는 함수를 준비하고
// 이 안에 각 객체를 넣는 것이다.
// 함수 속에서도 이들은 정적 객체로 선언하고
// 그 함수에서는 이들에 대한 참조자를 반환하게 만든다.

// 사용자는 비지역 정적 객체를 직접 참조하는
// 과거의 폐단을 버리고, 이제는 함수 호출로 대신한다.
// 즉, 비지역 정적 객체가. 지역 정적 객체. 로 바뀐 것이다. 

class FileSystem{} 

FileSystem& tfs() // tfs 객체를, 이 함수로 대신한다. 이 함수는 클래스 안에 정적 멤버로 들어가도 된다. 
{
    static FileSystem fs;
    return fs;
}

class Directory {...};

Directory::Directory(params)
{
    // tfs 의 참조자 , 가 tfs() 로 바뀌었다. 
    std::size_t disks = tfs().numDisks();
}

Directory& tempDir()
{
    // tempDir 객체를 이 함수로 대체한다.
    static Directory td;
    return td;
}

// tfs, tempDir 대신에 tfs() , tempDir() 을 참조하는 것으로 바꾼다.
// 즉, 정적 객체 자체를 직접 사용하지 않고, 그 객체에 대한 참조자를 반환하는 함수를 용하고 있는 것이다.


class FileSystem{}  // 지역 정적 객체 정의, 초기화 

FileSystem& tfs() 
{
    static FileSystem fs;
    return fs;
}

class Directory {...};

Directory::Directory(params)
{
    // tfs 의 참조자 , 가 tfs() 로 바뀌었다. 
    std::size_t disks = tfs().numDisks();
}

Directory& tempDir()
{
    // tempDir 객체를 이 함수로 대체한다.
    static Directory td;
    return td;
}
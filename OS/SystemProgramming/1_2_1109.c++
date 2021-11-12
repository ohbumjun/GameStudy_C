/*
https://www.inflearn.com/course/%EC%8B%9C%EC%8A%A4%ED%85%9C-%ED%94%84%EB%A1%9C%EA%B7%B8%EB%9E%98%EB%B0%8D/lecture/746?tab=note

Q1. 프로그램 실행 과정 4가지 --- 
1) 전처리기에 의한 치환 작업
ex) # Define

2) 컴파일러에 의한 번역
- C언어 --> 어셈블러 --> Binary 코드 생성

3) 어셈블러에 의한 바이너리 코드 생성
- CPU의 명령어를 
바이너리 코드로 번역 

4) 링커에 의한 연결과 결합
- 라이브러리와의 결합
- 어셈블러에 의해 만들어진 바이너리와
라이브러리가 합쳐져서
실행 파일이 만들어진다 
*/

/*
Q2. Stored Program Concept ?  --- 
- 프로그램이라는 것은, 
메모리 안에 저장되어야 한다는 것 
- 명령어는 메모리 안에 저장되어서
1) CPU에 의해, Fetch 되고
2) Decode 되고
3) Execute 되어야 한다
- 주로 by ALU

명령어가 메모리 안에 저장되어 있는 상태에서
cpu에 명령어를 하나씩 갖고 가서
명령어의 의미를 decode(해석) 하고
해석된 결과에 의해 실행(Execute) 시킨다

Q3. Stored Program Concept 그림 
그림 2개 
https://radical-pony-f47.notion.site/329161f4089f460893e9fdd9867e296e

Q4. 데이터 버스 (메모리 ~ CPU 관점) 3가지


*/
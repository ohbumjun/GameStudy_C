/*
비트단위 논리 연산자

진수
- 2, 8, 10, 16 진수

2진수
- 0 ~ 1 까지 

8진수
- 0 ~ 7

10 진수
- 0 ~9 

16 진수
- 0 ~ 15
-- 0 ~ 9 
-- 10 ~ 15 : a ~ f

진수들은, 서로 다른 진수로 변환이 가능하다

---
87을 2진수, 16진수로 변환한다.
87을 2진수로 변환한다 ?
- 계속 2로 나눠가면 된다
87 / 2 = 43 -- 1
43 / 2 = 21 -- 1
21 / 2 = 10 -- 1
10 / 2 = 5  -- 0
5   / 2 = 2  -- 1
2   / 2 = 1 -- 0

1010111

16진수로 변환
= 2진수로 변환, 그 다음에 16진수로 변환 시키기 

1010111 을 오른쪽에서부터 4자리씩 끊어서 계산한다.
16진수 1자리는 2진수 4자리이다.

0101 , 0111
8421   8421

2진수가 1인 부분의 값들을 더한다
	5    7

따라서 16진수로는 57 이 된다.

---

비트 단위 논리연산자란,
비트 단위, 즉, 2진수 단위연산을 하는 연산자.이다.

값대 값을 연산하여 값으로 나온다.
종류 : AND (&), OR(|), NOT(~), XOR(^ = 서로 다르면 1, 그외 0) , 

*/
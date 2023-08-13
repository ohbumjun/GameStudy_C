#include <iostream>
#include <list>

using namespace std;

int main()
{
	int n;
	n = 1;
	// 0 ~ 1
	// 

	// 0111  =  7
	// 1000  = -> 7의 보수를 취한다. (1의 보수) --> 
	// 1111  = 0 이 된다.  
	// 1111
  //   0000
  //   1111
	// 1001  = -> 1의 보수 + 1 = (2의 보수)

 //  10000  => 2의 보수 
  
	cout << ~n; 
}



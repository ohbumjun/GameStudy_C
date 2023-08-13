#include<iostream>
#include<new>
#include <set>
#include <vector>
#include <stack>
#include <map>
#include <string>

using namespace std;

/*
int main()
{
	1 - 2 = -1
	00000001  = 1
	00000010  = 2

	11111101  = -2 (?)
	11111111  = (x) 
	
	11111110 = -1

	// ��Ʈ ���� ( ���� ) + 1 == 2�� ���� 
	char c = 255; 

	// 2 - 2 => -1 --> + 1 == 0
	// 0 ~ 127    :
	// -128 ~ -1 : 1 byte 

	// ��ȣ ��Ʈ�� ���� 
	// 16���� 1�ڸ� = 2���� 1�ڸ� 
	
	// 2����, 8����, 16���� ���� --> ���� 
	printf("%d", c);
	return 0;
}
*/

void TwoToSixteen(std::vector<int>& bits);

bool CheckDigit(const char* str)
{
	for (int i = 0; i < strlen(str); i++)
	{
		if (!isdigit(str[i]))
			return false;
	}
	return true;
}

// 10���� --> �� 
void TenToSecond(int n)
{
	std::stack<int> bits;
	while (n >= 1)
	{
		bits.push(n % 2);
		n /= 2;
	}
	while (!bits.empty())
	{
		int num = bits.top();
		std::cout << num << "";
		bits.pop();
	}
}

std::vector<int> TenToTwoBit(int n)
{
	std::vector<int> bits;
	// 32�ڸ� ���� ǥ�� 
	for (int i = 31; i >= 0; i--)
	{
		if (n & (1 << i))
		{
			std::cout << 1 << "";
			bits.push_back(1);
		}
		else
		{
			std::cout << 0 << "";
			bits.push_back(0);
		}
	}
	return bits;
}

std::vector<string> TenToSixTeen(int n)
{
	// 2���� ����� 
	std::vector<int> bits;
	std::map<int, string> nums;
	std::vector<string> sixes;
	nums.insert(std::make_pair(10, "A"));
	nums.insert(std::make_pair(11, "B"));
	nums.insert(std::make_pair(12, "C"));
	nums.insert(std::make_pair(13, "D"));
	nums.insert(std::make_pair(14, "E"));
	nums.insert(std::make_pair(15, "F"));

	// 2���� ����� 
	for (int i = 31; i >= 0; i--)
	{
		if (n & (1 << i))
			bits.push_back(1);
		else
			bits.push_back(0);
	}


	for (int i = 0; i < 8; i++)
	{
		int sum = 0;
		int st	   = i * 4;
		int ed   = i * 4 + 3;
		int k     = 0;
		for (int j = ed; j >= st; j--)
		{
			if (bits[j] == 1)
				sum += pow(2,k);
			k += 1;
		}
		if (sum >= 10)
		{
			std::cout << nums[sum];
			sixes.push_back(nums[sum]);
		}
		else
		{
			std::cout << sum;
			sixes.push_back(std::to_string(sum));
		}
	}
	return sixes;
}

std::vector<int> TenToEight(int n)
{
	std::vector<int> bits;
	std::vector<int> eights;
	for (int i = 31; i >= 0; i--)
	{
		if (n & (1 << i))
			bits.push_back(1);
		else
			bits.push_back(0);
	}

	// 30�ڸ�����
	int stIdx = 2;
	for (int i = 0; i < 10; i++)
	{
		int st  = i * 3 + stIdx;
		int ed = st + 2;
		int k   = 0;
		int sum = 0;
		for (int j = ed; j >= st; j--)
		{
			if (bits[j] == 1)
				sum += pow(2, k);
			k += 1;
		}
		cout << sum;
		eights.push_back(sum);
	}
	return eights;
}

void SixTeenToTwo(std::vector<string> sixes)
{
	int diff = -1;
	std::vector<int> bits;
	std::stack<int> tmpStack;
	std::map<string, int> maps;
	maps.insert(make_pair("A", 10));
	maps.insert(make_pair("B", 11));
	maps.insert(make_pair("C", 12));
	maps.insert(make_pair("D", 13));
	maps.insert(make_pair("E", 14));
	maps.insert(make_pair("F", 15));

	for (int i = 0; i < sixes.size(); i++)
	{
		int curNum = -1;
		// �������� üũ
		if (CheckDigit(sixes[i].c_str()))
			curNum = std::stoi(sixes[i]);
		else
			curNum = maps[sixes[i]];

		// 2���� ���
		while (curNum >= 1)
		{
			tmpStack.push(curNum % 2);
			curNum /= 2;
		}

		diff = 4 - tmpStack.size();
		while (diff > 0)
		{
			bits.push_back(0);
			diff--;
		}

		// 2���� ��� ���缭 bits ��Ͽ� �ֱ� 
		while (!tmpStack.empty())
		{
			int curbit = tmpStack.top();
			tmpStack.pop();
			bits.push_back(curbit);
		}

	}

	for (const auto& b : bits)
		cout << b;
}

void SixTeenToEight(const std::vector<string>& sixes)
{
	int diff = -1;
	std::deque<int> bits;
	std::stack<int> stack;
	std::map<string, int> maps;
	maps.insert(std::make_pair("A", 10));
	maps.insert(std::make_pair("B", 11));
	maps.insert(std::make_pair("C", 12));
	maps.insert(std::make_pair("D", 13));
	maps.insert(std::make_pair("E", 14));
	maps.insert(std::make_pair("F", 15));

	// 2������ �ٲٱ� 
	for (size_t i = 0; i < sixes.size(); i++)
	{
		int curNum = -1;

		if (CheckDigit(sixes[i].c_str()))
			curNum = std::stoi(sixes[i]);
		else
			curNum = maps[sixes[i]];

		while (curNum >= 1)
		{
			stack.push(curNum % 2);
			curNum /= 2;
		}

		diff = 4 - stack.size();
		while (diff > 0)
		{
			bits.push_back(0);
			diff--;
		}

		while (!stack.empty())
		{
			bits.push_back(stack.top());
			stack.pop();
		}
	}
	


	// 2������ 8������ �ٲٱ�
	// 3�� ����� �Ǵ� ��ŭ �տ� 0�� �ֱ�
	while (bits.size() % 3 != 0)
	{
		bits.push_front(0);
	}

	// cout << "before" << endl;
	// for (const auto& b : bits)
		// cout << b << "";
	// cout << endl;

	// 8���� �ۼ��ϱ� 
	int eightLen = bits.size() / 3;
	for (int i = 0; i < eightLen; i++)
	{
		int st    = i * 3;
		int ed   = st + 2;
		int sum = 0;
		int k     = 0;
		for (int j = ed; j >= st; j--)
		{
			if (bits[j] == 1)
				sum += pow(2, k);
			k += 1;
		}
		cout << sum;
	}
}

void EightToTwo(const std::vector<int>& eights)
{
	// 8������ 2������ == 3�ڸ�
	int diff   = -1;
	int curN = -1;
	std::stack<int> stack;

	for (int i = 0; i < eights.size(); i++)
	{
		curN = eights[i];
		while (curN >= 1)
		{
			stack.push(curN % 2);
			curN /= 2;
		}

		diff = 3 - stack.size();
		while (diff > 0)
		{
			stack.push(0);
			diff--;
		}
		while (!stack.empty())
		{
			cout << stack.top();
			stack.pop();
		}
	}
	
}

void EightToSixteen(const std::vector<int>& eights)
{
	// 8������ 2������ == 3�ڸ�
	std::vector<int> bits;
	std::stack<int> stack;
	int diff = -1;
	int curN = -1;

	for (int i = 0; i < eights.size(); i++)
	{
		curN = eights[i];
		while (curN >= 1)
		{
			stack.push(curN % 2);
			curN /= 2;
		}

		diff = 3 - stack.size();
		while (diff > 0)
		{
			stack.push(0);
			diff--;
		}
		while (!stack.empty())
		{
			bits.push_back(stack.top());
			stack.pop();
		}
	}
	// 2������ ���� 16������ ����Ѵ�.
	TwoToSixteen(bits);
}

void TwoToEight(const std::vector<int>& bits)
{
	std::deque<int> bitsR;
	for (int i = 0; i < bits.size(); i++)
	{
		bitsR.push_back(bits[i]);
	}

	while (bitsR.size() % 3 != 0)
	{
		bitsR.push_front(0);
	}


	for (int i = 0; i < bitsR.size() / 3; i++)
	{
		int st = i * 3;
		int ed = st + 2;
		int sum = 0, k = 0;
		for (int j = ed; j >= st; j--)
		{
			if (bitsR[j] == 1)
				sum += pow(2, k);
			k += 1;
		}
		cout << sum; 
	}
}

void TwoToSixteen(std::vector<int>& bits)
{
	std::map<int, string> nums;
	std::deque<int> bistsR;
	nums.insert(std::make_pair(10, "A"));
	nums.insert(std::make_pair(11, "B"));
	nums.insert(std::make_pair(12, "C"));
	nums.insert(std::make_pair(13, "D"));
	nums.insert(std::make_pair(14, "E"));
	nums.insert(std::make_pair(15, "F"));

	// deque�� �����ϱ� 
	for (int i = 0; i < bits.size(); i++)
	{
		bistsR.push_back(bits[i]);
	}

	// 0���� ä��� 
	while (bistsR.size() % 4 != 0)
	{
		bistsR.push_front(0);
	}

	// 16���� ��ȯ
	int bitLen = bistsR.size();
	for (int i = 0; i < bitLen / 4; i++)
	{
		int st  = i * 4;
		int ed = st + 3;
		int sum = 0, k = 0;
		for (int j = ed; j >= st; j--)
		{
			if (bistsR[j] == 1)
				sum += pow(2, k);
			k += 1;
		}
		if (sum >= 10)
			cout << nums[sum];
		else
			cout << sum;
	}

}

int main()
{
	unsigned int input = 3;
	cout << "������ �Է��ϼ��� : ";
	cin >> input;

	// TenToSecond(input);
	cout << "2����             : ";
	std::vector<int>bits = TenToTwoBit(input);
	cout << endl;
	cout << "16����            : ";
	std::vector<string> sixes = TenToSixTeen(input);
	cout << endl;
	cout << "8����             : ";
	std::vector<int> eights = TenToEight(input);

	cout << endl;
	cout << "16������ 2������ : ";
	SixTeenToTwo(sixes);
		
	cout << endl;
	cout << "16������ 8������ : ";
	SixTeenToEight(sixes);

	cout << endl;
	cout << "8������ 2������   : ";
	EightToTwo(eights);

	cout << endl;
	cout << "8������ 16������ : ";
	EightToSixteen(eights);

	cout << endl;
	cout << "2������ 8������   : ";
	TwoToEight(bits);

	cout << endl;
	cout << "2������ 16������ : ";
	TwoToSixteen(bits);
	
	return 0;
}


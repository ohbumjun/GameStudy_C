#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <tchar.h>
#include <windows.h>

enum {
	DIV = 1,
	MUL,
	ADD,
	MIN,
	EXIT
};

DWORD ShowMenu(void);
BOOL Calculator(void);
DWORD FilterFunction(DWORD exptType);


int _tmain(int argc, TCHAR* argv[])
{
	BOOL state;

	do
	{
		state = Calculator();
	} while (state == TRUE);

	return 0;
};

DWORD ShowMenu(void)
{
	DWORD sel;

	_tprintf(_T("---------------------\n"));
	_tprintf(_T("1 Divide\n"));
	_tprintf(_T("2 Multiple\n"));
	_tprintf(_T("3 Add\n"));
	_tprintf(_T("4 Minus\n"));
	_tprintf(_T("5 Exit\n"));
	_tprintf(_T("SELECTION >> "));
	_tscanf(_T("%d"), &sel);
	
	return sel;
}

int num1, num2, result;

BOOL Calculator(void)
{
	DWORD sel;

	sel = ShowMenu();

	if (sel == EXIT)
		return FALSE;

	_tprintf(_T("Input num1, num2 : "));

	_tscanf(_T("%d %d"), &num1, &num2);

	__try
	{
		switch (sel)
		{
		case DIV :
		{
			_tprintf(_T("DIV\n"));
			result = num1 / num2;
			_tprintf(_T("num1 : %d, num2 : %d, result : %d"),  num1, num2, result);
			break;
		}
		case MUL:
		{
			_tprintf(_T("MUL\n"));
			result = num1 * num2;
			_tprintf(_T("num1 : %d, num2 : %d, result : %d"), num1, num2, result);
			break;
		}
		case ADD:
		{
			_tprintf(_T("ADD\n"));
			result = num1 + num2;
			_tprintf(_T("num1 : %d, num2 : %d, result : %d"), num1, num2, result);
			break;
		}
		case MIN:
		{
			_tprintf(_T("MIN\n"));
			result = num1 - num2;
			_tprintf(_T("num1 : %d, num2 : %d, result : %d"), num1, num2, result);
			break;
		}

		}
	}
	__except (FilterFunction(GetExceptionCode()))
	{
		_tprintf(_T("Except Block\n"));
	}

	return TRUE;
}

DWORD FilterFunction(DWORD exptType)
{
	switch (exptType)
	{
	case EXCEPTION_ACCESS_VIOLATION:
	{
		_tprintf(_T("Access Violation\n"));
		return EXCEPTION_EXECUTE_HANDLER;
	}
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
	{
		_tprintf(_T("Divide By Zero\n"));
		_tprintf(_T("ReInput num1, num2\n"));
		_tscanf(_T("%d %d"), &num1, &num2);

		return EXCEPTION_CONTINUE_EXECUTION;
	}
	default :
		return EXCEPTION_EXECUTE_HANDLER;
	}
}
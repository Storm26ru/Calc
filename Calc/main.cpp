#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include"resource.h"
#include"Dimensions.h"
#include<iostream>
//#include<stdio.h>

CONST CHAR* g_sz_MY_WINDOW_CLASS = "Calc_VPD_311";

CONST INT KEYPAD[4][5]{ {1007,1008,1009,1014,1015},{1004,1005,1006,1013,1016},{1001,1002,1003,1012,1017},{1000,0,1010,1011,0} };
CONST CHAR* CAPTION[4][5] = { {"7","8","9","/","<-"},{"4","5","6","*","C"},{"1","2","3","-","="},{"0","0",".","+","0"} };

INT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));
	wClass.style = 0;
	wClass.cbSize = sizeof(wClass);
	wClass.cbClsExtra = 0;
	wClass.cbWndExtra = 0;

	wClass.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
	wClass.hIconSm = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wClass.hbrBackground = CreateSolidBrush(RGB(217,228,241));

	wClass.hInstance = hInstance;
	wClass.lpszClassName = g_sz_MY_WINDOW_CLASS;
	wClass.lpszMenuName = NULL;
	wClass.lpfnWndProc = (WNDPROC)WndProc;
	if (!RegisterClassEx(&wClass))
	{
		MessageBox(NULL,"Class registration faled",NULL,MB_OK |MB_ICONERROR);
		return 0;
	}
	HWND hWnd = CreateWindowEx(NULL, g_sz_MY_WINDOW_CLASS, g_sz_MY_WINDOW_CLASS, WS_OVERLAPPEDWINDOW^WS_THICKFRAME^WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,g_i_WINDOW_WIDTH,
		g_i_WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
INT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static BOOL bOperation = FALSE;
	static BOOL bPoint = FALSE;
	switch (uMsg)
	{
	case WM_CREATE:
	{
		HWND hEdit = CreateWindowEx(NULL, "Edit", "0",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
			10, 10, g_i_SCREEN_WIDTH, g_i_SCREEN_HEIGHT,
			hWnd,
			(HMENU)IDC_EDIT_DISPLAY,
			GetModuleHandle(NULL),
			NULL);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				INT BUTTON_WIDTH = g_i_BUTTON_SIZE;
				INT BUTTON_HEIGHT = g_i_BUTTON_SIZE;
				if (!KEYPAD[i][j])continue;
				switch (KEYPAD[i][j])
				{
				case IDC_BUTTON_EQUAL:  BUTTON_HEIGHT += BUTTON_WIDTH + g_i_INTERVAL; break;
				case IDC_BUTTON_0:  BUTTON_WIDTH += BUTTON_HEIGHT + g_i_INTERVAL;
				}
				CreateWindowEx(NULL, "Button", CAPTION[i][j], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					g_i_BUTTON_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL) * j,
					g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * i,
					BUTTON_WIDTH, BUTTON_HEIGHT, hWnd, (HMENU)KEYPAD[i][j], GetModuleHandle(NULL), NULL);
			}
		}

	}break;
	case WM_COMMAND:
	{
		HWND hEdit = GetDlgItem(hWnd, IDC_EDIT_DISPLAY);
		CONST INT SIZE = 256;
		CHAR bufer[SIZE]{};
		CHAR digit[2]{};
		INT INDEX = SendMessage(hEdit, EM_LINELENGTH, 0, 0) - 1;
		SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)bufer);
		SendMessage(GetDlgItem(hWnd, LOWORD(wParam)), WM_GETTEXT, 2, (LPARAM)digit);
		if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_SLASH)
		{
			if (digit[0] >= 42 && digit[0] <= 47)
			{
				if (LOWORD(wParam) == IDC_BUTTON_POINT)
				{
					if (strchr(bufer, '.') && !bOperation)break;
					if (bPoint)
					{
						if (bPoint && strchr(bufer, '.') != strrchr(bufer, '.'))break;
					}
					else
					{
						if (strchr(bufer, '.'))break;
					}
				}

				if (bufer[INDEX] <= 45 || bufer[INDEX] == 47)
				{
					if (LOWORD(wParam) == IDC_BUTTON_POINT)
					{
						bufer[++INDEX] = '0';
						++INDEX;
					}
					bufer[INDEX] = digit[0];
					SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)bufer);
					break;
				}
				else
				{
					if (bOperation && LOWORD(wParam) != IDC_BUTTON_POINT)
					{
						SendMessage(hWnd, WM_COMMAND, LOWORD(IDC_BUTTON_EQUAL), 0);
						SendMessage(hWnd, WM_COMMAND, wParam, 0);
						break;
					}
					SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)strcat(bufer, digit));
					if (LOWORD(wParam) != IDC_BUTTON_POINT)
					{
						bOperation = TRUE;
						if (strchr(bufer, '.'))bPoint = TRUE;
					}
					break;
				}
			}
			if (strlen(bufer) == 1 && bufer[0] == '0')SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)digit);
			else SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)strcat(bufer, digit));
		}
		else
		{
			switch (LOWORD(wParam))
			{
			case IDC_BUTTON_CLR:
				bOperation = FALSE;
				bPoint = FALSE;
				ZeroMemory(bufer, SIZE);
				SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"0");
				break;
			case IDC_BUTTON_BSP:
				if (INDEX)
				{
					if (bufer[INDEX] <= 45 || bufer[INDEX] == 47)bOperation = FALSE;
					bufer[INDEX] = 0;
					SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)bufer);
				}
				else
				{
					bOperation = FALSE;
					ZeroMemory(bufer, SIZE);
					SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"0");
				}break;
			case IDC_BUTTON_EQUAL:
			{
				if (!bOperation)break;
				bOperation = FALSE;
				DOUBLE num[2]{}; 
				CHAR cOperator;
				strchr(bufer, '+') ? cOperator = '+' : strchr(bufer, '*') ? cOperator = '*' : strchr(bufer, '/') ? cOperator = '/' : cOperator = '-';
				if (bufer[INDEX] <= 45 || bufer[INDEX] == 47)
				{
					CHAR copy[SIZE]{};
					strcpy(copy, bufer);
					copy[INDEX] = 0;
					strcat(bufer, copy);
				}
				CHAR* token = strtok(bufer, "+-/*");
				INT j = 0;
				while (token)
				{
					num[j++] = atof(token);
					token = strtok(nullptr, "+-/*");
				}
				if (bufer[0] == '-')num[0] *= -1;
				switch (cOperator)
				{
				case '+': sprintf(bufer, "%g", num[0] + num[1]); break;
				case '-': sprintf(bufer, "%g", num[0] - num[1]); break;
				case '/': sprintf(bufer, "%g", num[0] / num[1]); break;
				case '*': sprintf(bufer, "%g", num[0] * num[1]);
				}
				SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)bufer);
			}break;
			}
		}
		SetFocus(hWnd);
	}
	break;
	case WM_KEYDOWN:
	switch (wParam)
	{
	case VK_ADD:
	case VK_OEM_PLUS:wParam = IDC_BUTTON_PLUS;break;
	case VK_SUBTRACT:
	case VK_OEM_MINUS:wParam = IDC_BUTTON_MINUS;break;
	case VK_MULTIPLY:wParam = IDC_BUTTON_ASTER; break;
	case VK_DIVIDE:
	case VK_OEM_2:wParam = IDC_BUTTON_SLASH; break;
	case VK_DECIMAL:
	case VK_OEM_PERIOD:wParam = IDC_BUTTON_POINT; break;
	case VK_BACK: wParam = IDC_BUTTON_BSP; break;
	case VK_ESCAPE: wParam = IDC_BUTTON_CLR; break;
	case VK_RETURN:wParam = IDC_BUTTON_EQUAL; break;
	}
		if (GetKeyState(VK_SHIFT) < 0)
		{
			if (wParam == '8') wParam = IDC_BUTTON_ASTER;
		}
		else
		{
			if (wParam >= '0' && wParam <= '9')wParam=wParam -'0' + IDC_BUTTON_0;
			if (wParam >= 0x60 && wParam <= 0x69)wParam=wParam - 0x60 + IDC_BUTTON_0;
		}
	SendMessage(hWnd, WM_COMMAND, wParam, 0); break;

	case WM_DESTROY: PostQuitMessage(0);
	case WM_CLOSE:DestroyWindow(hWnd);
	default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}
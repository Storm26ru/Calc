#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include"resource.h"
#include<iostream>

#define IDC_EDIT_DISPLAY	999

#define IDC_BUTTON_0		1000
#define IDC_BUTTON_1		1001
#define IDC_BUTTON_2		1002
#define IDC_BUTTON_3		1003
#define IDC_BUTTON_4		1004
#define IDC_BUTTON_5		1005
#define IDC_BUTTON_6		1006
#define IDC_BUTTON_7		1007
#define IDC_BUTTON_8		1008
#define IDC_BUTTON_9		1009
#define IDC_BUTTON_POINT	1010

#define IDC_BUTTON_PLUS		1011
#define IDC_BUTTON_MINUS	1012
#define IDC_BUTTON_ASTER	1013
#define IDC_BUTTON_SLASH	1014

#define IDC_BUTTON_BSP		1015
#define IDC_BUTTON_CLR		1016
#define IDC_BUTTON_EQUAL	1017

CONST CHAR* g_sz_MY_WINDOW_CLASS = "Calc_VPD_311";

CONST INT g_i_INTERVAL = 5;
CONST INT g_i_BUTTON_SIZE = 50;
CONST INT g_i_DBUTTON_SIZE = g_i_BUTTON_SIZE * 2 + g_i_INTERVAL;

CONST INT g_i_SCREEN_WIDTH = g_i_BUTTON_SIZE+(g_i_BUTTON_SIZE+g_i_INTERVAL)*4;
CONST INT g_i_SCREEN_HEIGHT = 30;

CONST INT g_i_START_X = 10;
CONST INT g_i_START_Y = 10;
CONST INT g_i_BUTTON_START_X = g_i_START_X;
CONST INT g_i_BUTTON_START_Y = g_i_START_Y + g_i_SCREEN_HEIGHT + g_i_INTERVAL;

CONST INT g_i_WINDOW_WIDTH = g_i_SCREEN_WIDTH + 36;
CONST INT g_i_WINDOW_HEIGHT = g_i_SCREEN_HEIGHT + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 4 + 58;
CONST INT KEYPAD[4][5]{ {1007,1008,1009,1014,1015},{1004,1005,1006,1013,1016},{1001,1002,1003,1012,1017},{1000,0,1010,1011,0} };
CONST CHAR* CAPTION[4][5] = { {"7","8","9","/","<-"},{"4","5","6","*","C"},{"1","2","3","-","="},{"0","0",",","+","0"} };

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
	HWND hWnd = CreateWindowEx(NULL, g_sz_MY_WINDOW_CLASS, g_sz_MY_WINDOW_CLASS, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,g_i_WINDOW_WIDTH,
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
		CONST INT SIZE = 256;
		CHAR bufer[SIZE]{};
		CHAR digit[2]{};
		INT INDEX = SendMessage(GetDlgItem(hWnd, IDC_EDIT_DISPLAY), EM_LINELENGTH, 0, 0) - 1;
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_DISPLAY), WM_GETTEXT, SIZE, (LPARAM)bufer);
		/*if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_9)
		{
			digit[0] = 48 + LOWORD(wParam) - IDC_BUTTON_0;
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_DISPLAY), WM_SETTEXT, 0, (LPARAM)strcat(bufer, digit));
		}*/
		if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_SLASH)
		{
			//digit[0] = 48 + LOWORD(wParam) - IDC_BUTTON_0;
			SendMessage(GetDlgItem(hWnd, LOWORD(wParam)), WM_GETTEXT, 2, (LPARAM)digit);
			if (digit[0] >= 42 && digit[0] <= 47)
			{
				if (bufer[0] == '0')break;
				if (bufer[INDEX] >= 42 && bufer[INDEX] <= 47)
				{
					bufer[INDEX] = digit[0];
					SendMessage(GetDlgItem(hWnd, IDC_EDIT_DISPLAY), WM_SETTEXT, 0, (LPARAM)bufer);
					break;
				}
			}
			if (bufer[0] == '0')SendMessage(GetDlgItem(hWnd, IDC_EDIT_DISPLAY), WM_SETTEXT, 0, (LPARAM)digit);
			else SendMessage(GetDlgItem(hWnd, IDC_EDIT_DISPLAY), WM_SETTEXT, 0, (LPARAM)strcat(bufer, digit));
		}
		else
		{
			switch (LOWORD(wParam))
			{
			case IDC_BUTTON_CLR: SendMessage(GetDlgItem(hWnd, IDC_EDIT_DISPLAY), WM_SETTEXT, 0, (LPARAM)"0"); break;
			case IDC_BUTTON_BSP:
				if (INDEX)
				{
					bufer[INDEX] = 0; 
					SendMessage(GetDlgItem(hWnd, IDC_EDIT_DISPLAY), WM_SETTEXT, 0, (LPARAM)bufer);
				}else SendMessage(GetDlgItem(hWnd, IDC_EDIT_DISPLAY), WM_SETTEXT, 0, (LPARAM)"0"); break;
			}
		}

	}
	break;
	case WM_DESTROY: PostQuitMessage(0);
	case WM_CLOSE:DestroyWindow(hWnd);
	default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}
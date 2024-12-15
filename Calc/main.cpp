#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include"resource.h"
#include"Dimensions.h"
#include<float.h>
#include<cstdio>
#include<windowsx.h>

CONST CHAR* g_sz_WINDOW_CLASS = "Calc_VPD_311";

CONST INT KEYPAD[4][5]{ {1007,1008,1009,1014,1015},{1004,1005,1006,1013,1016},{1001,1002,1003,1012,1017},{1000,0,1010,1011,0} };
CONST CHAR* CAPTION[4][5] = { {"7","8","9","/","<-"},{"4","5","6","*","C"},{"1","2","3","-","="},{"0","0",".","+","0"} };
CONST CHAR* g_OPERATION[]{ "+","-","*","/" };
CONST COLORREF EDIT_BACKGROUND[] = { RGB(0,0, 240), RGB(0,  128, 64) };
CONST COLORREF FONT[] = { RGB(255, 255, 255), RGB(255, 0, 0) };
CONST COLORREF WIN_BACKGROUND[] = { RGB(0, 0, 150), RGB(75,  75, 75) };
CONST CHAR* File[]{ "square_blue","metal_mistral" };

INT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID SetSkin(HWND hWnd, CONST CHAR* skin);


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));
	wClass.style = 0;
	wClass.cbSize = sizeof(wClass);
	wClass.cbClsExtra = 24;
	wClass.cbWndExtra = 24;

	wClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wClass.hbrBackground = CreateSolidBrush(RGB(159, 156, 148));

	wClass.hInstance = hInstance;
	wClass.lpszClassName = g_sz_WINDOW_CLASS;
	wClass.lpszMenuName = NULL;
	wClass.lpfnWndProc = (WNDPROC)WndProc;
	if (!RegisterClassEx(&wClass))
	{
		MessageBox(NULL, "Class registration faled", NULL, MB_OK | MB_ICONERROR);
		return 0;
	}

	HWND hWnd = CreateWindowEx(NULL, g_sz_WINDOW_CLASS, g_sz_WINDOW_CLASS, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, g_i_WINDOW_WIDTH,
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
	
	static INT color_index = 0;
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
		
		AddFontResourceEx("Fonts\\Calc_2.ttf", FR_PRIVATE, 0);//https://learn.microsoft.com/ru-ru/windows/win32/gdi/font-and-text-functions
		AddFontResourceEx("Fonts\\Pocket_Calculator.ttf", FR_PRIVATE, 0);
		HFONT hFont = CreateFont(32, 12, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_TT_ALWAYS, CLEARTYPE_QUALITY, FF_DONTCARE, "DiamanteSerial-Light");
		SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);

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
				CreateWindowEx(NULL, "Button", CAPTION[i][j], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
					g_i_BUTTON_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL) * j,
					g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * i,
					BUTTON_WIDTH, BUTTON_HEIGHT, hWnd, (HMENU)KEYPAD[i][j], GetModuleHandle(NULL), NULL);
			}
		}
		//SetSkin(hWnd, "square_blue");
		//SetSkin(hWnd, "metal_mistral");
	}break;
	case WM_COMMAND:
	{
		static DOUBLE a = DBL_MIN;
		static DOUBLE b = DBL_MIN;
		static WORD operation = 0;
		static BOOL input = FALSE;
		static BOOL operation_input = FALSE;
		static BOOL equal_pressed = FALSE;
		HWND hEditDisplay = GetDlgItem(hWnd, IDC_EDIT_DISPLAY);
		CONST INT SIZE = 256;
		CHAR sz_display[SIZE]{};
		CHAR sz_digit[2]{};
		SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display);
		if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_9)
		{
			if (operation_input || equal_pressed)
			{
				//SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)"");
				strcpy(sz_display, "");
				operation_input = equal_pressed = FALSE;
			}
			sz_digit[0] = LOWORD(wParam) - IDC_BUTTON_0 + '0';
			if (strlen(sz_display) == 1 && sz_display[0] == '0') sz_display[0] = sz_digit[0];
			else strcat(sz_display, sz_digit);
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
			input = TRUE;
		}
		if (LOWORD(wParam) == IDC_BUTTON_POINT)
		{
			if (strchr(sz_display, '.'))break;
			strcat(sz_display, ".");
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
		if (LOWORD(wParam) == IDC_BUTTON_BSP)
		{
			if (strlen(sz_display) > 1)sz_display[strlen(sz_display) - 1] = 0;
			else sz_display[0] = '0';
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
		if (LOWORD(wParam) == IDC_BUTTON_CLR)
		{
			a = b = DBL_MIN;
			operation = 0;
			input = operation_input = FALSE;
			ZeroMemory(sz_display, sizeof(sz_display));
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)"0");
		}
		if (LOWORD(wParam) >= IDC_BUTTON_PLUS && LOWORD(wParam) <= IDC_BUTTON_SLASH)
		{
			if (input && a == DBL_MIN)a = atof(sz_display);
			if (operation && input) SendMessage(hWnd, WM_COMMAND, LOWORD(IDC_BUTTON_EQUAL), 0);
			operation = LOWORD(wParam);
			operation_input = TRUE;
			equal_pressed = FALSE;
		}
		if (LOWORD(wParam) == IDC_BUTTON_EQUAL)
		{
			if (input)b = atof(sz_display);
			input = FALSE;
			switch (operation)
			{
			case IDC_BUTTON_PLUS: a += b; break;
			case IDC_BUTTON_MINUS: a -= b; break;
			case IDC_BUTTON_ASTER: a *= b; break;
			case IDC_BUTTON_SLASH: a /= b; break;
			}
			operation_input = FALSE;
			equal_pressed = TRUE;
			sprintf(sz_display, "%g", a);
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
		}

		SetFocus(hWnd);
	}break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ADD:
		case VK_OEM_PLUS:wParam = IDC_BUTTON_PLUS; break;
		case VK_SUBTRACT:
		case VK_OEM_MINUS:wParam = IDC_BUTTON_MINUS; break;
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
			if (wParam >= '0' && wParam <= '9')wParam = wParam - '0' + IDC_BUTTON_0;
			if (wParam >= 0x60 && wParam <= 0x69)wParam = wParam - 0x60 + IDC_BUTTON_0;
		}
		SendMessage(hWnd, WM_COMMAND, wParam, 0);
		break;
	case WM_CONTEXTMENU:
	{
		HMENU hSubMenuSkin = CreatePopupMenu();
		InsertMenu(hSubMenuSkin, 0, MF_BYPOSITION | MF_STRING, IDR_SQUARE_BLUE, "Square_blue");
		InsertMenu(hSubMenuSkin, 1, MF_BYPOSITION | MF_STRING, IDR_METAL_MISTRAL, "Metal_mistral");
		HMENU hSubMenuFont = CreatePopupMenu();
		InsertMenu(hSubMenuFont, 0, MF_BYPOSITION | MF_STRING, IDR_DIAMANTE_SERIAL_LIGHT, "Diamante_Serial_Light");
		InsertMenu(hSubMenuFont, 1, MF_BYPOSITION | MF_STRING, IDR_Pocket_CALCULATOR, "Pocket_Calculator");
		HMENU hMenu = CreatePopupMenu();
		InsertMenu(hMenu, 0, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hSubMenuSkin, "Skin");
		InsertMenu(hMenu, 1, MF_BYPOSITION | MF_SEPARATOR, 0, 0);
		InsertMenu(hMenu, 2, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hSubMenuFont, "Font");
		BOOL skin_index = TrackPopupMenuEx(hMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD,
			GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), hWnd, NULL);
		switch (skin_index)
		{
		case IDR_SQUARE_BLUE:SetSkin(hWnd, "square_blue");break;
		case IDR_METAL_MISTRAL: SetSkin(hWnd, "metal_mistral"); break;
		case IDR_DIAMANTE_SERIAL_LIGHT:
		{
			HFONT hFont = CreateFont(32, 12, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_TT_ALWAYS, CLEARTYPE_QUALITY, FF_DONTCARE, "DiamanteSerial-Light");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_DISPLAY), WM_SETFONT, (WPARAM)hFont, TRUE);
		}break;
		case IDR_Pocket_CALCULATOR:
		{
			HFONT hFont = CreateFont(32, 12, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_TT_ALWAYS, CLEARTYPE_QUALITY, FF_DONTCARE, "Pocket Calculator");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_DISPLAY), WM_SETFONT, (WPARAM)hFont, TRUE);

		}
		
		}
			DestroyMenu(hSubMenuSkin);
			DestroyMenu(hSubMenuFont);
			DestroyMenu(hMenu);
		if (skin_index >= IDR_SQUARE_BLUE && skin_index <= IDR_METAL_MISTRAL)
		{
			color_index = skin_index - IDR_SQUARE_BLUE;

			HWND hEditDisplay = GetDlgItem(hWnd, IDC_EDIT_DISPLAY);
			HDC hdcDisplay = GetDC(hEditDisplay);
			//SendMessage(hWnd, WM_CTLCOLOREDIT, (WPARAM)GetDC(GetDlgItem(hWnd, IDC_EDIT_DISPLAY)), (LPARAM)GetDlgItem(hWnd, IDC_EDIT_DISPLAY));
			SendMessage(hWnd, WM_CTLCOLOREDIT, (WPARAM)hdcDisplay, (LPARAM)hEditDisplay);
			ReleaseDC(hEditDisplay, hdcDisplay);
		}
	}break;
	case WM_CTLCOLOREDIT:
	{
		//if (GetDlgCtrlID((HWND)lParam)==IDC_EDIT_DISPLAY);
		//{
		//	SetTextColor((HDC)wParam, RGB(0, 255, 0));
		//	SetBkColor((HDC)wParam, RGB(41, 41, 41));
		//	//SetSkin(hWnd, "square_blue");
		//	HBRUSH hbrBackground = CreateSolidBrush(RGB(0, 98,161));
		//	SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(0, 98, 161)));
		//	/*InvalidateRect(GetDlgItem(hWnd, IDC_EDIT_DISPLAY), NULL, TRUE);
		//	UpdateWindow(hWnd);*/
		//	SendMessage(hWnd, WM_ERASEBKGND, wParam, 0);
		//	SetFocus(GetDlgItem(hWnd, IDC_EDIT_DISPLAY));
		//	return (LRESULT)hbrBackground;
		//}
	
		//if (GetDlgItem(hWnd, IDC_EDIT_DISPLAY) ==(HWND) lParam);
		if (GetDlgCtrlID((HWND)lParam) == IDC_EDIT_DISPLAY);
		{
			SetTextColor((HDC)wParam, FONT[color_index]);
			SetBkColor((HDC)wParam, EDIT_BACKGROUND[color_index]);
			SetSkin(hWnd, File[color_index]);
			HBRUSH hbrBackground = CreateSolidBrush(WIN_BACKGROUND[color_index]);
			SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG)CreateSolidBrush(WIN_BACKGROUND[color_index]));
			SendMessage(hWnd, WM_ERASEBKGND, wParam, 0);
			SetFocus(GetDlgItem(hWnd, IDC_EDIT_DISPLAY));
			return (LRESULT)hbrBackground;
		}

	}break;
	case WM_DESTROY: 
	{
		HWND hEdit = GetDlgItem(hWnd, IDC_EDIT_DISPLAY);
		HDC hdc = GetDC(hEdit);
		ReleaseDC(hEdit, hdc);
		PostQuitMessage(0);
	}
	case WM_CLOSE: DestroyWindow(hWnd);
	default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return FALSE;
}

CONST CHAR* button_image[]{ "button_0","button_1","button_2","button_3","button_4","button_5",
							"button_6","button_7","button_8","button_9","button_point","button_plus",
							"button_minus","button_aster","button_slash","button_bsp","button_clr","button_equal" };

VOID SetSkin(HWND hWnd, CONST CHAR* skin)
{
	CHAR Filename[FILENAME_MAX]{}; https://learn.microsoft.com/ru-ru/cpp/c-runtime-library/filename-max?view=msvc-17
for (int i = 0; i < 18; i++)
{
	sprintf(Filename, "ButtonBMP\\%s\\%s.bmp", skin, button_image[i]);
	HWND hButton = GetDlgItem(hWnd, IDC_BUTTON_0 + i);
	HBITMAP hImage = (HBITMAP)LoadImage(NULL, Filename, IMAGE_BITMAP,
		i + IDC_BUTTON_0 == IDC_BUTTON_0 ? g_i_BUTTON_SIZE * 2 + g_i_INTERVAL : g_i_BUTTON_SIZE,
		i + IDC_BUTTON_0 == IDC_BUTTON_EQUAL ? g_i_BUTTON_SIZE * 2 + g_i_INTERVAL : g_i_BUTTON_SIZE, LR_LOADFROMFILE);
	SendMessage(hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage);
}
}
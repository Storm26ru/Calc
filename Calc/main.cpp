#include<Windows.h>
#include"resource.h"
#include"Dimensions.h"

CONST CHAR* g_sz_WINDOW_CLASS = "Calc_VPD_311";
INT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));
	wClass.style = 0;
	wClass.cbSize = sizeof(wClass);
	wClass.cbClsExtra = 0;
	wClass.cbWndExtra = 0;

	wClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wClass.hbrBackground = CreateSolidBrush(RGB(217, 228, 241));

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
	switch (uMsg)
	{
	case WM_CREATE:break;
	case WM_COMMAND:break;
	case WM_DESTROY: PostQuitMessage(0);
	case WM_CLOSE: DestroyWindow(hWnd);
	default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return FALSE;
}

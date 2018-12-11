#define _USE_MATH_DEFINES
#include <windows.h>
#include <math.h>
#include <gdiplus.h>

#define BUTTON_PLAY_ID 10000
#define BUTTON_EXIT_ID 10001

char szFile[MAX_PATH];
int posX = 1;
int posY = 1;

const int speed = 10;
const int BoardHeight = 100;
const int BoardWidth = 100;
const int WindowHeight = 700;
const int WindowWidth = 1200;

void Exit(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	int MB_RESULT = MessageBox(hWnd, "Do you really want to exit ?", "Exit", MB_YESNO);
	if (MB_RESULT == 6)
		SendMessage(hWnd, WM_DESTROY, wParam, lParam);
}

//message handler
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	static HBRUSH solidBrush = CreateSolidBrush(RGB(0, 255, 0));

	PAINTSTRUCT ps;
	int fwKeys;
	int MB_RESULT;
	
	switch (message)
	{	
	case WM_COMMAND:
		switch (wParam)
		{
		case BUTTON_EXIT_ID:
			Exit(hWnd, wParam, lParam);
			break;
		}
		break;
	//message from keybord
	case WM_KEYDOWN: // Обработка нажатия клавиши
		if (wParam == 68 || wParam == 39) //вправо
			posX += speed;
		if (wParam == 65 || wParam == 37) //влево
			posX -= speed;
		if (wParam == 83 || wParam == 40) //вниз
			posY += speed;
		if (wParam == 87 || wParam == 38) //вверх
			posY -= speed;
		if (wParam == 27) //если нажали ESC то выходим 
		{
			Exit(hWnd, wParam, lParam);
		}

		InvalidateRect(hWnd, NULL, TRUE);
		break;

	//repaint message
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		RECT rect;
		rect.top = posY;
		rect.left = posX;
		rect.right = posX + BoardWidth;
		rect.bottom = posY + BoardHeight;
		FillRect(hdc, &rect, solidBrush);
			
		ReleaseDC(hWnd, hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		DeleteObject(solidBrush);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	HWND buttonPlay, buttonExit;
	WNDCLASSEX wcex;
	HWND hWnd;
	MSG msg;

	//window class initialization
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_CROSS);//IDC_ARROW
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOWTEXT + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "PongGame";
	wcex.hIconSm = wcex.hIcon;
	RegisterClassEx(&wcex);//window registration

	//window creation
	hWnd = CreateWindow("PongGame", "The Game Pong",
		WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	
	buttonPlay = CreateWindow("button", "PLAY", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 500, 10, 120, 30, hWnd, (HMENU)BUTTON_PLAY_ID, hInstance, NULL);
	buttonExit = CreateWindow("button", "EXIT", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 500, 100, 120, 30, hWnd, (HMENU)BUTTON_EXIT_ID, hInstance, NULL);
		
	HDC hdc = GetDC(buttonPlay);
	HFONT hFont = CreateFont(30, 30, 0, 0, FW_DONTCARE, FALSE, FALSE,
		FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH,
		TEXT("Times New Roman"));
	SendMessage(buttonPlay, WM_SETFONT,(WPARAM) hFont, true);
	ReleaseDC(buttonPlay, hdc);

	//set window size
	MoveWindow(hWnd, 150, 100, WindowWidth, WindowHeight, NULL);
	//show window
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//recieve message
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}
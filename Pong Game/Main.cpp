#define _USE_MATH_DEFINES
#include <windows.h>
#include <math.h>
#include <gdiplus.h>

#define BUTTON_PLAY_ID 10000
#define BUTTON_HELP_ID 10001
#define BUTTON_EXIT_ID 10002

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

void SetButtonFont(HWND hWnd,HFONT hFont)
{
	HDC hdc = GetDC(hWnd);
	SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, true);
	ReleaseDC(hWnd, hdc);
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
		case BUTTON_HELP_ID:
			MessageBox(hWnd, "gg \nnoob", "HELP", MB_OK);
			break;
		case BUTTON_EXIT_ID:
			Exit(hWnd, wParam, lParam);
			break;
		}
		break;
	//message from keybord
	case WM_KEYDOWN: // ��������� ������� �������
		if (wParam == 68 || wParam == 39) //������
			posX += speed;
		if (wParam == 65 || wParam == 37) //�����
			posX -= speed;
		if (wParam == 83 || wParam == 40) //����
			posY += speed;
		if (wParam == 87 || wParam == 38) //�����
			posY -= speed;
		if (wParam == 27) //���� ������ ESC �� ������� 
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
	HWND buttonPlay, buttonExit, buttonHelp;
	WNDCLASSEX wcex;
	HWND hWnd;
	MSG msg;

	HFONT buttonFont = CreateFont(30, 20, 0, 0, FW_DONTCARE, FALSE, FALSE,
		FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH,
		TEXT("Times New Roman"));

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
	
	//buttons creation
	buttonPlay = CreateWindow("button", "PLAY", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, WindowWidth/2-125, WindowHeight/2-100-70, 250, 100, hWnd, (HMENU)BUTTON_PLAY_ID, hInstance, NULL);
	buttonHelp = CreateWindow("button", "HELP", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, WindowWidth/2-125, WindowHeight/2-50, 250, 100, hWnd, (HMENU)BUTTON_HELP_ID, hInstance, NULL);
	buttonExit = CreateWindow("button", "EXIT", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, WindowWidth/2-125, WindowHeight/2+50+20, 250, 100, hWnd, (HMENU)BUTTON_EXIT_ID, hInstance, NULL);
		
	//set buttons font
	SetButtonFont(buttonPlay, buttonFont);
	SetButtonFont(buttonHelp, buttonFont);
	SetButtonFont(buttonExit, buttonFont);

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
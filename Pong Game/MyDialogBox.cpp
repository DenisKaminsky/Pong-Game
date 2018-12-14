#include "MyDialogBox.h"

WNDCLASSEX wcex;
HWND leftButton;
HWND rightButton;

int inclineButton;
int wWidth = 400;
int wHeight = 400;

void CreateButtons(HWND hWnd, HINSTANCE hInstance, int buttonWidth, int buttonHeight, int Incline)
{
	inclineButton = Incline;

	leftButton = CreateWindow("button", "Left", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_PUSHBUTTON | BS_OWNERDRAW, (wWidth / 2) - buttonWidth - 50 , wHeight / 2 + buttonHeight, buttonHeight, buttonHeight, hWnd, (HMENU)BUTTON_LEFT, hInstance, NULL);
	rightButton = CreateWindow("button", "Right", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_PUSHBUTTON | BS_OWNERDRAW, wWidth / 2 + buttonWidth + 50, wHeight / 2 + buttonHeight, buttonWidth, buttonHeight, hWnd, (HMENU)BUTTON_RIGHT, hInstance, NULL);
}

//message handler
LRESULT CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	static HBRUSH solidBrush = CreateSolidBrush(RGB(0, 255, 0));
	PAINTSTRUCT ps;
	int MB_RESULT;

	switch (message)
	{
		//ownerdraw buttons
	case WM_DRAWITEM:
	{
		switch (wParam)
		{
		case BUTTON_LEFT:
			//RedrawButton(buttonPlay, "PLAY", 4);
			break;
		case BUTTON_RIGHT:
			//RedrawButton(buttonHelp, "HELP", 4);
			break;
		}
	}
	break;
	//buttons commands
	case WM_COMMAND:
		switch (wParam)
		{
		case BUTTON_LEFT:
			break;
		case BUTTON_RIGHT:
			break;
		}
		break;
	//close window
	case WM_CLOSE:
		MB_RESULT = MessageBox(hWnd, "Do you really want to exittttt ?", "Exittt", MB_YESNO);
		if (MB_RESULT == 6)
		{
			ShowWindow(hWnd, SW_HIDE);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

HWND CreateDialogBox(HINSTANCE hInstance)
{
	HWND hWnd;

	//window class initialization
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_DBLCLKS;
	wcex.lpfnWndProc = DialogProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_CROSS);//IDC_ARROW
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOWTEXT + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "MyDialogBox";
	wcex.hIconSm = wcex.hIcon;
	RegisterClassEx(&wcex);//window registration

	//window creation
	hWnd = CreateWindow("MyDialogBox", "Dialog",
		WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	//font creation
	/*HFONT hFont = CreateFont(60, 30, 0, 0, FW_DONTCARE, FALSE, FALSE,
		FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH,
		TEXT("SlantCYRILLIC"));*/

	//buttons creation
	//InitializeButtons(hWnd, hInstance, 260, 80, 30);
	//set window size
    MoveWindow(hWnd, 150, 100, wWidth,wHeight, NULL);
	//recieve message
	return hWnd;
}
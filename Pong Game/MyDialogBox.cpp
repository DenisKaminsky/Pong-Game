#include "MyDialogBox.h"
#include "Tools.h"

#define BUTTON_YES 8000
#define BUTTON_NO 8001
#define BUTTON_OK 8002
#define BUTTON_CONTINUE 8003

WNDCLASSEX wcex;
HWND yesButton,noButton;
HWND okButton,continueButton;
HWND parent;

int inclineButton;
int wWidth = 500;
int wHeight = 250;

void CreateButtons(HWND hWnd, HINSTANCE hInstance, int buttonWidth, int buttonHeight, int Incline)
{
	inclineButton = Incline;

	yesButton = CreateWindow("button", "YES", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_PUSHBUTTON | BS_OWNERDRAW, 20, wHeight / 2 , buttonWidth, buttonHeight, hWnd, (HMENU)BUTTON_YES, hInstance, NULL);
	noButton = CreateWindow("button", "NO", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_PUSHBUTTON | BS_OWNERDRAW, wWidth - buttonWidth - 20, wHeight / 2, buttonWidth, buttonHeight, hWnd, (HMENU)BUTTON_NO, hInstance, NULL);
	okButton = CreateWindow("button", "OK", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_PUSHBUTTON | BS_OWNERDRAW, wWidth/2 - buttonWidth/2, wHeight / 2, buttonWidth, buttonHeight, hWnd, (HMENU)BUTTON_OK, hInstance, NULL);
	continueButton = CreateWindow("button", "CONTINUE", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_PUSHBUTTON | BS_OWNERDRAW, wWidth / 2 - (buttonWidth+15)/2, wHeight / 2, buttonWidth+15, buttonHeight, hWnd, (HMENU)BUTTON_CONTINUE, hInstance, NULL);
}

void ShowDialog(HWND hWndDialog,HWND hWndParent)
{
	parent = hWndParent;
	EnableWindow(parent, FALSE);
	ShowWindow(hWndDialog,SW_SHOW);
}

void ShowPauseDialog(HWND hWndDialog, HWND hWndParent)
{
	InvalidateRect(hWndDialog, NULL, TRUE);
	SendMessage(hWndDialog, WM_SETTEXT, 0, (LPARAM)"PAUSE");
	ShowWindow(yesButton, SW_HIDE);
	ShowWindow(noButton, SW_HIDE);
	ShowWindow(okButton, SW_HIDE);
	ShowWindow(continueButton, SW_SHOW);
	ShowDialog(hWndDialog, hWndParent);
	HFONT hFont = CreateFont(60, 30, 0, 0, FW_DONTCARE, FALSE, FALSE,
		FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH,
		TEXT("SlantCYRILLIC"));
	HDC hdc = GetDC(hWndDialog);

	PrintTextToScreen(hdc, wWidth / 2 - 80, 10, "PAUSE", 5, RGB(253, 196, 18), hFont);
	ReleaseDC(hWndDialog, hdc);
	DeleteObject(hFont);
}

void ShowHelpDialog(HWND hWndDialog, HWND hWndParent)
{
	InvalidateRect(hWndDialog, NULL, TRUE);
	SendMessage(hWndDialog, WM_SETTEXT, 0, (LPARAM)"HELP");
	ShowWindow(yesButton, SW_HIDE);
	ShowWindow(noButton, SW_HIDE);
	ShowWindow(okButton, SW_SHOW);
	ShowWindow(continueButton, SW_HIDE);
	ShowDialog(hWndDialog, hWndParent);
	HFONT hFont = CreateFont(30, 15, 0, 0, FW_DONTCARE, FALSE, FALSE,
		FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH,
		TEXT("SlantCYRILLIC"));
	HDC hdc = GetDC(hWndDialog);

	PrintTextToScreen(hdc, wWidth / 2 - 200, 10, "LEFT PLAYER CONTROLS BY W/S", 27, RGB(0, 255, 0), hFont);
	PrintTextToScreen(hdc, wWidth / 2 - 230, 40, "RIGHT PLAYER CONTROLS BY ARROWS", 31, RGB(0, 255, 0), hFont);
	PrintTextToScreen(hdc, wWidth / 2 - 235, 70, "WINNER IS THE PLAYER WITH 6 POINTS", 34, RGB(0, 255, 0), hFont);
	ReleaseDC(hWndDialog, hdc);
	DeleteObject(hFont);
}

void ShowDialogExitProgram(HWND hWndDialog, HWND hWndParent)
{
	InvalidateRect(hWndDialog, NULL, TRUE);
	SendMessage(hWndDialog, WM_SETTEXT, 0, (LPARAM)"EXIT");
	ShowWindow(yesButton, SW_SHOW);
	ShowWindow(noButton, SW_SHOW);
	ShowWindow(okButton, SW_HIDE);
	ShowWindow(continueButton, SW_HIDE);
	ShowDialog(hWndDialog, hWndParent);
	HFONT hFont = CreateFont(30, 15, 0, 0, FW_DONTCARE, FALSE, FALSE,
		FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH,
		TEXT("SlantCYRILLIC"));
	HDC hdc = GetDC(hWndDialog);

	PrintTextToScreen(hdc, wWidth / 2 - 190, 50, "DO YOU REALLY WANT TO EXIT?", 27, RGB(255, 0, 0), hFont);
	ReleaseDC(hWndDialog, hdc);
	DeleteObject(hFont);
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
		case BUTTON_YES:
			RedrawButton(yesButton, "YES", 3,30);
			break;
		case BUTTON_NO:
			RedrawButton(noButton, "NO", 2,30);
			break;
		case BUTTON_OK:
			RedrawButton(okButton, "OK", 2, 30);
			break;
		case BUTTON_CONTINUE:
			RedrawButton(continueButton, "CONTINUE", 8, 30);
			break;
		}
	}
	break;
	//buttons commands
	case WM_COMMAND:
		switch (wParam)
		{
		case BUTTON_YES:
			SendMessage(parent, WM_DESTROY, NULL, NULL);
			break;
		case BUTTON_CONTINUE:
		case BUTTON_OK:
		case BUTTON_NO:
			EnableWindow(parent, TRUE);
			ShowWindow(hWnd, SW_HIDE);
			break;
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void SetWindowSize(HWND hWnd)
{
	HDC hDCScreen = GetDC(NULL);
	int HLen = GetDeviceCaps(hDCScreen, HORZRES);
	int VLen = GetDeviceCaps(hDCScreen, VERTRES);
	ReleaseDC(NULL, hDCScreen);
	MoveWindow(hWnd, HLen/2-wWidth/2, VLen/2-wHeight/2, wWidth, wHeight, NULL);

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
		WS_OVERLAPPED, CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	//font creation
	HFONT hFont = CreateFont(60, 30, 0, 0, FW_DONTCARE, FALSE, FALSE,
		FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH,
		TEXT("SlantCYRILLIC"));

	//set window size
	SetWindowSize(hWnd);
	//buttons creation
	CreateButtons(hWnd, hInstance, 130, 80, 30);
	//recieve message
	return hWnd;
}
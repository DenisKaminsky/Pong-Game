#define _USE_MATH_DEFINES
#include <windows.h>
#include <math.h>
#include "Graphics.h"
#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")

#define MAIN_MENU_ID 1
#define MODE_MENI_ID 2
#define DIFFICULTY_MENU_ID 3
#define RESULT_MENU_ID 4

#define BUTTON_PLAY_ID 10000
#define BUTTON_HELP_ID 10001
#define BUTTON_EXIT_ID 10002
#define BUTTON_ONE_PLAYER_ID 10003
#define BUTTON_TWO_PLAYER_ID 10004
#define BUTTON_EASY_ID 10005
#define BUTTON_NORMAL_ID 10006
#define BUTTON_HARD_ID 10007
#define BUTTON_REPEAT_ID 10008
#define BUTTON_MAIN_MENU_ID 10009

HWND buttonPlay, buttonExit, buttonHelp;//main menu button
HWND buttonOnePlayer, buttonTwoPlayer; //choose mode menu
HWND buttonEasy, buttonNormal, buttonHard;//choose difficulty
HWND buttonRepeat, buttonMainMenu; //game result;
HWND dialogBox;
HFONT hFont;
HBITMAP hBitmap;

int currentMenuId;

int posX = 1;
int posY = 1;

const int speed = 10;
const int BoardHeight = 100;
const int BoardWidth = 100;
const int WindowHeight = 700;
const int WindowWidth = 1200;
int buttonIncline;

void SetButtonFont(HWND hWnd,HFONT hFont)
{
	HDC hdc = GetDC(hWnd);
	SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, true);
	ReleaseDC(hWnd, hdc);
}

SIZE GetBitmapSize(HBITMAP hBitmap)
{
	BITMAP bitmap;
	GetObject(hBitmap, sizeof(BITMAP), &bitmap);
	SIZE result;
	result.cx = bitmap.bmWidth;
	result.cy = bitmap.bmHeight;
	return result;
}

HBITMAP LoadImageToBitmap(LPCWSTR path)
{
	HBITMAP hBitmap;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);//инифиализируем gdi+

	Gdiplus::Bitmap *img = Gdiplus::Bitmap::FromFile(path);
	img->GetHBITMAP(GetSysColor(COLOR_WINDOW), &hBitmap);
	Gdiplus::GdiplusShutdown(gdiplusToken);
	return hBitmap;
}

//create form of button
HRGN CreateButtonForm(int buttonWidth, int buttonHeight,int offset)
{
	HRGN region;
	POINT *pnt = (POINT *)malloc(sizeof(POINT) * 6);
	pnt[0].x = offset;
	pnt[0].y = buttonHeight;
	pnt[1].x = buttonWidth - offset;
	pnt[1].y = buttonHeight;
	pnt[2].x = buttonWidth;
	pnt[2].y = buttonHeight/2;
	pnt[3].x = buttonWidth - offset;
	pnt[3].y = 0;
	pnt[4].x = offset;
	pnt[4].y = 0;
	pnt[5].x = 0;
	pnt[5].y = buttonHeight/2;
	region = CreatePolygonRgn(pnt, 6, ALTERNATE);
	free(pnt);
	return region;
}

void PrintTextToScreen(HDC hdc, int x, int y, LPCSTR text, int length, COLORREF color,HFONT hFont)
{	
	SelectObject(hdc, hFont);
	SetTextColor(hdc, color);
	SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, x, y, text, length);
}

void ShowMainMenu(HWND hWnd)
{
	HDC hdc;

	InvalidateRect(hWnd, NULL, TRUE);
	currentMenuId = MAIN_MENU_ID;
	ShowWindow(buttonPlay, SW_SHOW);
	ShowWindow(buttonHelp, SW_SHOW);
	ShowWindow(buttonExit, SW_SHOW);
	ShowWindow(buttonOnePlayer, SW_HIDE);
	ShowWindow(buttonTwoPlayer, SW_HIDE);
	ShowWindow(buttonEasy, SW_HIDE);
	ShowWindow(buttonNormal, SW_HIDE);
	ShowWindow(buttonHard, SW_HIDE);
	ShowWindow(buttonRepeat, SW_HIDE);
	ShowWindow(buttonMainMenu, SW_HIDE);
	
	hdc = GetDC(hWnd);
	PrintTextToScreen(hdc, WindowWidth / 2 - 200, 100, "THE GAME PONG", 13, RGB(0, 0, 255),hFont);
	ReleaseDC(hWnd, hdc);	
}

void ShowChooseModeMenu(HWND hWnd)
{
	HDC hdc;

	InvalidateRect(hWnd, NULL, TRUE);
	currentMenuId = MODE_MENI_ID;
	ShowWindow(buttonPlay, SW_HIDE);
	ShowWindow(buttonHelp, SW_HIDE);
	ShowWindow(buttonExit, SW_HIDE);
	ShowWindow(buttonOnePlayer, SW_SHOW);
	ShowWindow(buttonTwoPlayer, SW_SHOW);
	ShowWindow(buttonEasy, SW_HIDE);
	ShowWindow(buttonNormal, SW_HIDE);
	ShowWindow(buttonHard, SW_HIDE);

	hdc = GetDC(hWnd);
	PrintTextToScreen(hdc, WindowWidth / 2 - 80, 140, "MODE", 4, RGB(0, 255, 0),hFont);
	ReleaseDC(hWnd, hdc);
}

void ShowDifficultyMenu(HWND hWnd)
{
	HDC hdc;

	InvalidateRect(hWnd, NULL, TRUE);
	currentMenuId = DIFFICULTY_MENU_ID;
	ShowWindow(buttonOnePlayer, SW_HIDE);
	ShowWindow(buttonTwoPlayer, SW_HIDE);
	ShowWindow(buttonEasy, SW_SHOW);
	ShowWindow(buttonNormal, SW_SHOW);
	ShowWindow(buttonHard, SW_SHOW);	

	hdc = GetDC(hWnd);
	PrintTextToScreen(hdc, WindowWidth / 2 - 125, 120, "DIFFICULTY", 10, RGB(255, 0, 0),hFont);
	ReleaseDC(hWnd, hdc);
}

void ShowGameResult(HWND hWnd,LPCSTR gameResult,int length)
{
	HDC hdc, sDC;
	HGDIOBJ prevobj;
	SIZE bitmapSize;

	InvalidateRect(hWnd, NULL, TRUE);
	currentMenuId = RESULT_MENU_ID;
	ShowWindow(buttonPlay, SW_HIDE);
	ShowWindow(buttonHelp, SW_HIDE);
	ShowWindow(buttonExit, SW_HIDE);
	ShowWindow(buttonRepeat, SW_SHOW);
	ShowWindow(buttonMainMenu, SW_SHOW);

	hdc = GetDC(hWnd);
	PrintTextToScreen(hdc, WindowWidth / 2 - (length*28)/2 , 220, gameResult, length, RGB(255, 0, 0),hFont);
	sDC = CreateCompatibleDC(hdc);//создаем контекст устройства в памяти , совместимый с данным устройством
	prevobj = SelectObject(sDC, hBitmap);
	bitmapSize = GetBitmapSize(hBitmap);
	BitBlt(hdc, 320, 30, bitmapSize.cx, bitmapSize.cy, sDC, 0, 0, SRCCOPY);
	SelectObject(sDC, prevobj);
	DeleteDC(sDC);
	ReleaseDC(hWnd, hdc);
}

void InitializeButtons(HWND hWnd, HINSTANCE hInstance, int buttonWidth, int buttonHeight, int Incline)
{
	buttonIncline = Incline;
	//menu buttons
	buttonPlay = CreateWindow("button", "PLAY", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_PUSHBUTTON | BS_OWNERDRAW, WindowWidth / 2 - buttonWidth/2, WindowHeight / 2 - buttonHeight - (buttonHeight/2 + 20), buttonWidth, buttonHeight, hWnd, (HMENU)BUTTON_PLAY_ID, hInstance, NULL);
	buttonHelp = CreateWindow("button", "HELP", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_PUSHBUTTON | BS_OWNERDRAW, WindowWidth / 2 - buttonWidth / 2, WindowHeight / 2 - buttonHeight/2, buttonWidth, buttonHeight, hWnd, (HMENU)BUTTON_HELP_ID, hInstance, NULL);
	buttonExit = CreateWindow("button", "EXIT", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_PUSHBUTTON | BS_OWNERDRAW, WindowWidth / 2 - buttonWidth / 2, WindowHeight / 2 + buttonHeight/2 + 20, buttonWidth, buttonHeight, hWnd, (HMENU)BUTTON_EXIT_ID, hInstance, NULL);
	//mode buttons
	buttonOnePlayer = CreateWindow("button", "ONE PLAYER",  WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_PUSHBUTTON | BS_OWNERDRAW, WindowWidth / 2 - buttonWidth / 2, WindowHeight / 2 - buttonHeight - 20, buttonWidth, buttonHeight, hWnd, (HMENU)BUTTON_ONE_PLAYER_ID, hInstance, NULL);
	buttonTwoPlayer = CreateWindow("button", "TWO PLAYERS", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_PUSHBUTTON | BS_OWNERDRAW, WindowWidth / 2 - buttonWidth / 2, WindowHeight / 2, buttonWidth, buttonHeight, hWnd, (HMENU)BUTTON_TWO_PLAYER_ID, hInstance, NULL);
	//difficulty buttons
	buttonEasy = CreateWindow("button", "EASY", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_PUSHBUTTON | BS_OWNERDRAW, WindowWidth / 2 - buttonWidth / 2, WindowHeight / 2 - buttonHeight - (buttonHeight / 2 + 20), buttonWidth, buttonHeight, hWnd, (HMENU)BUTTON_EASY_ID, hInstance, NULL);
	buttonNormal = CreateWindow("button", "NORMAL", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_PUSHBUTTON | BS_OWNERDRAW, WindowWidth / 2 - buttonWidth / 2, WindowHeight / 2 - buttonHeight/2, buttonWidth, buttonHeight, hWnd, (HMENU)BUTTON_NORMAL_ID, hInstance, NULL);
	buttonHard = CreateWindow("button", "HARD", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_PUSHBUTTON | BS_OWNERDRAW, WindowWidth / 2 - buttonWidth / 2, WindowHeight / 2 + buttonHeight/2 + 20, buttonWidth, buttonHeight, hWnd, (HMENU)BUTTON_HARD_ID, hInstance, NULL);
	//gameover buttons
	buttonRepeat = CreateWindow("button", "REPEAT", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_PUSHBUTTON | BS_OWNERDRAW, WindowWidth / 2 - buttonWidth / 2, WindowHeight / 2 - buttonHeight/2 , buttonWidth, buttonHeight, hWnd, (HMENU)BUTTON_REPEAT_ID, hInstance, NULL);
	buttonMainMenu = CreateWindow("button", "MAIN MENU", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_PUSHBUTTON | BS_OWNERDRAW, WindowWidth / 2 - buttonWidth / 2, WindowHeight / 2 + buttonHeight/2 + 20, buttonWidth, buttonHeight, hWnd, (HMENU)BUTTON_MAIN_MENU_ID, hInstance, NULL);
}

void RedrawButton(HWND button,LPCSTR text,int textLength)
{
	RECT rect,rectForText;
	HRGN region;
	HFONT buttonFont = CreateFont(30, 15, 0, 0, FW_DONTCARE, FALSE, FALSE,
		FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH,
		TEXT("SlantCYRILLIC"));

	HDC hdc = GetDC(button);
	//get button form
	GetWindowRect(button, &rect);
	region = CreateButtonForm(rect.right-rect.left,rect.bottom-rect.top,buttonIncline);
	rectForText.top = 0;
	rectForText.left = 0;
	rectForText.right = 1;
	rectForText.bottom = 1;
	//create button form
	SelectObject(hdc, buttonFont);
	//get text length
	DrawText(hdc, text, textLength, &rectForText, DT_CALCRECT | DT_SINGLELINE);
	SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, (rect.right-rect.left)/2- (rectForText.right)/2, (rect.bottom-rect.top)/2-15, text, textLength);
	SetWindowRgn(button, region, true);
	UpdateWindow(button);
	ReleaseDC(button, hdc);
	DeleteObject(buttonFont);
}

//message handler
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
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
		case BUTTON_PLAY_ID:
			RedrawButton(buttonPlay, "PLAY", 4);
			break;
		case BUTTON_HELP_ID:
			RedrawButton(buttonHelp, "HELP", 4);
			break;
		case BUTTON_EXIT_ID:
			RedrawButton(buttonExit, "EXIT", 4);
			break;
		case BUTTON_ONE_PLAYER_ID:
			RedrawButton(buttonOnePlayer, "ONE PLAYER", 10);
			break;
		case BUTTON_TWO_PLAYER_ID:
			RedrawButton(buttonTwoPlayer, "TWO PLAYERS", 11);
			break;
		case BUTTON_EASY_ID:
			RedrawButton(buttonEasy, "EASY", 4);
			break;
		case BUTTON_NORMAL_ID:
			RedrawButton(buttonNormal, "NORMAL", 6);
			break;
		case BUTTON_HARD_ID:
			RedrawButton(buttonHard, "HARD", 4);
			break;
		case BUTTON_REPEAT_ID:
			RedrawButton(buttonRepeat, "REPEAT", 6);
			break;
		case BUTTON_MAIN_MENU_ID:
			RedrawButton(buttonMainMenu, "MAIN MENU", 9);
			break;
		}	
	}
	break;
	//buttons commands
	case WM_COMMAND:
		switch (wParam)
		{
		case BUTTON_PLAY_ID:
			ShowChooseModeMenu(hWnd);
			break;
		case BUTTON_ONE_PLAYER_ID:
			ShowDifficultyMenu(hWnd);
			break;
		case BUTTON_TWO_PLAYER_ID:			
			break;
		case BUTTON_EASY_ID:
			//SetFocus(hWnd);
			break;
		case BUTTON_NORMAL_ID:
			//SetFocus(hWnd);
			break;
		case BUTTON_HARD_ID:
			ShowWindow(dialogBox, SW_SHOW);
			//SetFocus(hWnd);
			break;
		case BUTTON_HELP_ID:
			MessageBox(hWnd, "gg \nnoob", "HELP", MB_OK);			
			break;
		case BUTTON_EXIT_ID:
			SendMessage(hWnd, WM_CLOSE, wParam, lParam);
			break;
		case BUTTON_REPEAT_ID:
			break;
		case BUTTON_MAIN_MENU_ID:
			ShowMainMenu(hWnd);
			break;
		}
		break;
	//message from keybord
	case WM_KEYDOWN: // Обработка нажатия клавиши
	{
		bool needUpdate = false;
		if (wParam == 83 || wParam == 40) //вниз
			posY += speed;
		if (wParam == 87 || wParam == 38) //вверх
			posY -= speed;
		if (wParam == 27) //если нажали ESC то выходим 
		{
			//SendMessage(hWnd, WM_CLOSE, wParam, lParam);
		}
		if (wParam == 8) //press BackSpace to Go Back
		{
			switch (currentMenuId) {
			case MODE_MENI_ID:
				ShowMainMenu(hWnd);
				break;
			case DIFFICULTY_MENU_ID:
				ShowChooseModeMenu(hWnd);
				break;
			}
		}
		if (needUpdate)
			InvalidateRect(hWnd, NULL, TRUE);
	}
	break;
	//repaint message
	/*case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		RECT rect;
		rect.top = posY;
		rect.left = posX;
		rect.right = posX + BoardWidth;
		rect.bottom = posY + BoardHeight;
		FillRect(hdc, &rect, solidBrush);

		ReleaseDC(hWnd, hdc);
		EndPaint(hWnd, &ps);
		break;*/
	//closing window
	case WM_CLOSE:
		MB_RESULT = MessageBox(hWnd, "Do you really want to exit ?", "Exit", MB_YESNO);
		if (MB_RESULT == 6)
			SendMessage(hWnd, WM_DESTROY, wParam, lParam);
		break;
	//destroy window
	case WM_DESTROY:
	{
		DeleteObject(solidBrush);
		DeleteObject(hFont);
		PostQuitMessage(0);
	}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{	
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
	
	hBitmap = LoadImageToBitmap(L"..\\ResourceFiles\\game.jpg");

	//window creation
	hWnd = CreateWindow("PongGame", "The Game Pong",
		WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	
	//font creation
	hFont = CreateFont(60, 30, 0, 0, FW_DONTCARE, FALSE, FALSE,
		FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH,
		TEXT("SlantCYRILLIC"));

	//buttons creation
	InitializeButtons(hWnd, hInstance,260,80,30);
	//set window size
	MoveWindow(hWnd, 150, 100, WindowWidth, WindowHeight, NULL);	
	
	//show window
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	//show menu
	ShowMainMenu(hWnd);
	ShowGameResult(hWnd, "PLAYER 1 WIN", 12);
	//recieve message
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
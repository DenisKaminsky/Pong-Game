#pragma once
#include <Windows.h>

void SetButtonFont(HWND hWnd, HFONT hFont);
SIZE GetBitmapSize(HBITMAP hBitmap);
HBITMAP LoadImageToBitmap(LPCWSTR path);
HRGN CreateButtonForm(int buttonWidth, int buttonHeight, int offset);
void PrintTextToScreen(HDC hdc, int x, int y, LPCSTR text, int length, COLORREF color, HFONT hFont);
void RedrawButton(HWND button, LPCSTR text, int textLength, int buttonIncline);

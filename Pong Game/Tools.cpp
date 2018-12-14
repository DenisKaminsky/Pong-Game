#include "Tools.h"
#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")

void SetButtonFont(HWND hWnd, HFONT hFont)
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

HRGN CreateButtonForm(int buttonWidth, int buttonHeight, int offset)
{
	HRGN region;
	POINT *pnt = (POINT *)malloc(sizeof(POINT) * 6);
	pnt[0].x = offset;
	pnt[0].y = buttonHeight;
	pnt[1].x = buttonWidth - offset;
	pnt[1].y = buttonHeight;
	pnt[2].x = buttonWidth;
	pnt[2].y = buttonHeight / 2;
	pnt[3].x = buttonWidth - offset;
	pnt[3].y = 0;
	pnt[4].x = offset;
	pnt[4].y = 0;
	pnt[5].x = 0;
	pnt[5].y = buttonHeight / 2;
	region = CreatePolygonRgn(pnt, 6, ALTERNATE);
	free(pnt);
	return region;
}

void PrintTextToScreen(HDC hdc, int x, int y, LPCSTR text, int length, COLORREF color, HFONT hFont)
{
	SelectObject(hdc, hFont);
	SetTextColor(hdc, color);
	SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, x, y, text, length);
}

void RedrawButton(HWND button, LPCSTR text, int textLength, int buttonIncline)
{
	RECT rect, rectForText;
	HRGN region;
	HFONT buttonFont = CreateFont(30, 15, 0, 0, FW_DONTCARE, FALSE, FALSE,
		FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH,
		TEXT("SlantCYRILLIC"));

	HDC hdc = GetDC(button);
	//get button form
	GetWindowRect(button, &rect);
	region = CreateButtonForm(rect.right - rect.left, rect.bottom - rect.top, buttonIncline);
	rectForText.top = 0;
	rectForText.left = 0;
	rectForText.right = 1;
	rectForText.bottom = 1;
	//create button form
	SelectObject(hdc, buttonFont);
	//get text length
	DrawText(hdc, text, textLength, &rectForText, DT_CALCRECT | DT_SINGLELINE);
	SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, (rect.right - rect.left) / 2 - (rectForText.right) / 2, (rect.bottom - rect.top) / 2 - 15, text, textLength);
	SetWindowRgn(button, region, true);
	UpdateWindow(button);
	ReleaseDC(button, hdc);
	DeleteObject(buttonFont);
}
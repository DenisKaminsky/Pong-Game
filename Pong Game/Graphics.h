#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <wincodec.h>

class Graphics
{
	ID2D1Factory* factory;
	ID2D1HwndRenderTarget* renderTarget;//where to draw
	ID2D1Bitmap* bitmap;
public:
	Graphics();//constructor
	~Graphics();//destructor

	bool Init(HWND windowHandle);
	void BeginDraw() { renderTarget->BeginDraw(); }
	void EndDraw() { renderTarget->EndDraw(); }
	void ClearScreen(float r, float g, float b);
	void DrawCircle(float x, float y, float radius, float r, float g, float b,float a);
	void DrawRoundRectangle(float x, float y, float width, float height, float r, float g, float b, float a);
	void DrawRectangle(float x, float y, float width, float height, float r, float g, float b, float a);
	void DrawString(const wchar_t* text,int length, float x, float y, float width, float height,float font, float r, float g, float b, float a);
	void LoadImageFromFile(LPWSTR fileName);
	void DrawImage(float x, float y, float width, float height);
};
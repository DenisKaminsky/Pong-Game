#pragma once

#include <Windows.h>
#include <d2d1.h>

class Graphics
{
	ID2D1Factory* factory;
	ID2D1HwndRenderTarget* renderTarget;//where to draw
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
	void DrawString(LPCWSTR text,int length, float x, float y, float width, float height,float fontSize, float r, float g, float b, float a);
};
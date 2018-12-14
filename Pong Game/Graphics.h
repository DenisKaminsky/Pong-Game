#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <wincodec.h>

class Graphics
{
	ID2D1Factory* factory;
	ID2D1HwndRenderTarget* renderTarget;//where to draw
	ID2D1Bitmap* bitmapBackground;
	ID2D1Bitmap* bitmapLBoard;
	ID2D1Bitmap* bitmapRBoard;
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
	ID2D1Bitmap* LoadImageFromFile(LPWSTR fileName);
	void DrawImage(ID2D1Bitmap* bitmap,float x, float y, float width, float height);
	void SetBitmapBackground(ID2D1Bitmap* bitmap);
	void SetBitmapLBoard(ID2D1Bitmap* bitmap);
	void SetBitmapRBoard(ID2D1Bitmap* bitmap);
	ID2D1Bitmap* GetBitmapBackground();
	ID2D1Bitmap* GetBitmapLBoard();
	ID2D1Bitmap* GetBitmapRBoard();
};
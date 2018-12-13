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
};
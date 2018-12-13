#include "Graphics.h"

Graphics::Graphics()
{
	factory = NULL;
	renderTarget = NULL;
}

Graphics::~Graphics()
{
	if (factory) factory->Release();
	if (renderTarget) renderTarget->Release();
}

//init factory and renderTarget
bool Graphics::Init(HWND windowHandle)
{
	RECT rect;
	HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);//creaye single thread factory
	
	if (res != S_OK)
		return false;

	GetClientRect(windowHandle, &rect);
	//bind our window with D2
	res = factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			windowHandle, D2D1::SizeU(rect.right, rect.bottom)),
		&renderTarget
	);

	if (res != S_OK)
		return false;
	return true;
}

void Graphics::ClearScreen(float r, float g, float b)
{
	renderTarget->Clear(D2D1::ColorF(r, g, b));
}

void Graphics::DrawCircle(float x, float y, float radius, float r, float g, float b,float a)
{
	ID2D1SolidColorBrush* brush;

	renderTarget->CreateSolidColorBrush(D2D1::ColorF(r, g, b, a), &brush);
	renderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), brush);
	//renderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x,y),radius,radius),brush,3.0f);
	brush->Release();
}

void Graphics::DrawRectangle(float x, float y,float width,float height, float r, float g, float b, float a)
{
	ID2D1SolidColorBrush* brush;

	renderTarget->CreateSolidColorBrush(D2D1::ColorF(r, g, b, a), &brush);
	renderTarget->FillRoundedRectangle(D2D1::RoundedRect(D2D1::Rect(x,y,x+width,y+height),5,5), brush);
	//renderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x,y),radius,radius),brush,3.0f);
	brush->Release();
}


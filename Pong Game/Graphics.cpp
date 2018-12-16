#include "Graphics.h"
#include <dwrite.h>
#pragma comment(lib,"dwrite.lib")

using namespace std;

Graphics::Graphics()
{
	factory = NULL;
	renderTarget = NULL;
	bitmapBackground = NULL;
	bitmapLBoard = NULL;
	bitmapLBoard = NULL;
}

Graphics::~Graphics()
{
	if (factory) factory->Release();
	if (renderTarget) renderTarget->Release();
	if (bitmapBackground) bitmapBackground->Release();
	if (bitmapLBoard) bitmapLBoard->Release();
	if (bitmapRBoard) bitmapRBoard->Release();
}

//init factory and renderTarget
bool Graphics::Init(HWND windowHandle)
{
	RECT rect;
	HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);//create single thread factory
	
	if (res != S_OK)
		return false;

	GetClientRect(windowHandle, &rect);
	//bind our window with D2
	res = factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_IGNORE)),
		D2D1::HwndRenderTargetProperties(
			windowHandle, D2D1::SizeU(rect.right, rect.bottom)),
		&renderTarget
	);
	renderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
	if (res != S_OK)
		return false;
	return true;
}

void Graphics::SetBitmapBackground(ID2D1Bitmap* bitmap)
{
	bitmapBackground = bitmap;
}

void Graphics::SetBitmapLBoard(ID2D1Bitmap* bitmap)
{
	bitmapLBoard = bitmap;
}

void Graphics::SetBitmapRBoard(ID2D1Bitmap* bitmap)
{
	bitmapRBoard = bitmap;
}

ID2D1Bitmap* Graphics::GetBitmapBackground()
{
	return bitmapBackground;
}

ID2D1Bitmap* Graphics::GetBitmapLBoard()
{
	return bitmapLBoard;
}

ID2D1Bitmap* Graphics::GetBitmapRBoard()
{
	return bitmapRBoard;
}

//redraw region
void Graphics::ClearScreen(float r, float g, float b)
{
	renderTarget->Clear(D2D1::ColorF(r, g, b));
}

void Graphics::DrawCircle(float x, float y, float radius, float r, float g, float b,float a)
{
	ID2D1SolidColorBrush* brush;

	renderTarget->CreateSolidColorBrush(D2D1::ColorF(r, g, b, a), &brush);
	renderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), brush);
	brush->Release();
}

void Graphics::DrawRoundRectangle(float x, float y,float width,float height, float r, float g, float b, float a)
{
	ID2D1SolidColorBrush* brush;

	renderTarget->CreateSolidColorBrush(D2D1::ColorF(r, g, b, a), &brush);
	renderTarget->FillRoundedRectangle(D2D1::RoundedRect(D2D1::Rect(x,y,x+width,y+height),5,5), brush);
	brush->Release();
}

void Graphics::DrawRectangle(float x, float y, float width, float height, float r, float g, float b, float a)
{
	ID2D1SolidColorBrush* brush;

	renderTarget->CreateSolidColorBrush(D2D1::ColorF(r, g, b, a), &brush);
	renderTarget->FillRectangle(D2D1::Rect(x, y, x + width, y + height), brush);
	brush->Release();
}

void Graphics::DrawString(const wchar_t* text,int length,float x,float y,float width,float height,float fontSize,float r, float g, float b, float a)
{
	ID2D1SolidColorBrush* brush;
	IDWriteFactory* dwFactory;
	IDWriteTextFormat* format;	
	
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(dwFactory), reinterpret_cast<IUnknown **>(&dwFactory));
	dwFactory->CreateTextFormat(L"SlantCYRILLIC", nullptr, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontSize, L"en-us", &format);
	format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(r, g, b, a), &brush);
	renderTarget->DrawTextA(text, length,format, D2D1::Rect(x, y, x + width, y + height), brush);
	brush->Release();
	format->Release();
	dwFactory->Release();
}

ID2D1Bitmap* Graphics::LoadImageFromFile(LPWSTR fileName)//bitmap is ID2D1Bitmap where we save our picture
{
	IWICImagingFactory* WICFactory;
	IWICBitmapDecoder *WICDecoder = NULL;
	IWICBitmapFrameDecode *pFrame = NULL;
	IWICFormatConverter *WICConverter = NULL;
	ID2D1Bitmap* bmp = NULL;
	HRESULT hr;

	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&WICFactory);
	hr = WICFactory->CreateDecoderFromFilename(
		fileName,                      // Image to be decoded
		NULL,                            // Do not prefer a particular vendor
		GENERIC_READ,                    // Desired read access to the file
		WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
		&WICDecoder                        // Pointer to the decoder
	);
	// Retrieve the first frame of the image from the decoder
	if (SUCCEEDED(hr))
	{
		hr = WICDecoder->GetFrame(0, &pFrame);
	}
	// Format convert the frame to 32bppPBGRA
	if (SUCCEEDED(hr))
	{
		hr = WICFactory->CreateFormatConverter(&WICConverter);
	}
	if (SUCCEEDED(hr))
	{
		hr = WICConverter->Initialize(
			pFrame,                          // Input bitmap to convert
			GUID_WICPixelFormat32bppPBGRA,   // Destination pixel format
			WICBitmapDitherTypeNone,         // Specified dither pattern
			NULL,                            // Specify a particular palette 
			0.0f,                             // Alpha threshold
			WICBitmapPaletteTypeCustom       // Palette translation type
		);
	}
	if (WICConverter && !bmp)
	{
		renderTarget->CreateBitmapFromWicBitmap(WICConverter, NULL, &bmp);
	}
	
	WICFactory->Release();
	WICDecoder->Release();
	WICConverter->Release();
	pFrame->Release();
	//bmp = NULL;
	return bmp;
}

void Graphics::DrawImage(ID2D1Bitmap* bitmap, float x, float y, float width, float height)
{	
	if (bitmap)
	{
		renderTarget->DrawBitmap(bitmap, D2D1::Rect(x, y, x + width, y + height));
	}
}


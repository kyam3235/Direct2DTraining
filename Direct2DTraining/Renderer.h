//Renderer.h

//TODO:Win7 Direct2D sample1ÇéQçlÇ…çÏê¨Ç∑ÇÈ
#pragma once
class Renderer
{
private:
	ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	ID2D1SolidColorBrush* m_pBackgroundBrush;
	ID2D1SolidColorBrush* m_pSmallStarBrush;

public:
	Renderer(HWND hwnd, int width, int height);
	~Renderer(void);

	HRESULT Init();
	HRESULT Render();
	void DiscardResources();

	HWND m_hwnd;
	bool m_animate;
	int m_clickedPointX;
	int m_clickedPointY;

private:
	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources();
};
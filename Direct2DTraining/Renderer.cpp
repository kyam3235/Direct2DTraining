//Renderer.cpp

#include "stdafx.h"
#include "Renderer.h"

//コンストラクタ
Renderer::Renderer(HWND hwnd, int width, int height)
{
	this->m_hwnd = hwnd;
	this->m_pDirect2dFactory = NULL;
	this->m_pRenderTarget = NULL;
	this->m_animate = false;
	this->CreateDeviceIndependentResources();
}

//デストラクタ
Renderer::~Renderer(void)
{
	this->DiscardResources();
}

HRESULT Renderer::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);
	return hr;
}

HRESULT Renderer::CreateDeviceResources()
{
	HRESULT hr = S_OK;
	if (!this->m_pRenderTarget)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
		hr = this->m_pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(this->m_hwnd, size),
			&m_pRenderTarget
			);
		if (SUCCEEDED(hr))
		{
			//背景用ブラシの作成
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF(0x007DD5)),
				&m_pBackgroundBrush
				);
		}
		if (SUCCEEDED(hr))
		{
			//星描画用ブラシの作成
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::White),
				&m_pSmallStarBrush
				);
		}
		return hr;
	}
}

//メインとなる描画関数
HRESULT Renderer::Render()
{
	HRESULT hr = S_OK;
	hr = this->CreateDeviceResources();
	if (SUCCEEDED(hr))
	{
		//描画開始の合図
		this->m_pRenderTarget->BeginDraw();

		this->m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		this->m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	}
	if (SUCCEEDED(hr))
	{
		D2D1_SIZE_F size = m_pRenderTarget->GetSize();
		D2D1_RECT_F rectBackground = D2D1::RectF(0, 0, size.width, size.height);

		//RenderTargetのサイズを取得する
		float rtWidth = this->m_pRenderTarget->GetSize().width;
		float rtHeight = this->m_pRenderTarget->GetSize().height;

		//星の描画
		for (int i = 0; i < 50000; i++)
		{
			int x = (float)(rand()) / RAND_MAX * rtWidth;
			int y = (float)(rand()) / RAND_MAX * rtHeight;
			D2D1_ELLIPSE smallStar = D2D1_ELLIPSE();
			smallStar.point = D2D1::Point2F(x, y);
			smallStar.radiusX = 1;
			smallStar.radiusY = 1;
			this->m_pRenderTarget->FillEllipse(&smallStar, this->m_pSmallStarBrush);
		}

		if (this->m_animate)
		{
			InvalidateRect(this->m_hwnd, NULL, FALSE);
		}

		hr = (this->m_pRenderTarget->EndDraw());
	}
	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardResources();
	}
	return hr;
}

void Renderer::DiscardResources()
{
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pBackgroundBrush);
	SafeRelease(&m_pSmallStarBrush);
	SafeRelease(&m_pDirect2dFactory);
}

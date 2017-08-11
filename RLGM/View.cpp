#include "stdafx.h"
#include "View.h"
#include "Interface.h"

namespace RLGM
{
	
	View::View()
	{

	}
	View::View(HWND HWnd, RLGM::Interface* Owner)
	{
		hWnd = HWnd;
		owner = Owner;
		
		RECT rc;
		GetClientRect(hWnd, &rc);
		width = rc.right - rc.left;
		height = rc.bottom - rc.top;		

		aspectRatio = height / (float)width;

		InitializeGraphics();
	}
	//Инициализация Графики
	void View::InitializeGraphics()
	{
		HRESULT hr = S_OK;

		// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
		IDXGIFactory* dxgiFactory = nullptr;
		{
			IDXGIDevice* dxgiDevice = nullptr;
			hr = owner->g_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
			if (SUCCEEDED(hr))
			{
				IDXGIAdapter* adapter = nullptr;
				hr = dxgiDevice->GetAdapter(&adapter);
				if (SUCCEEDED(hr))
				{
					hr = adapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory));
					adapter->Release();
				}
				dxgiDevice->Release();
			}
		}
		if (FAILED(hr)) return;// hr;

		// Create swap chain
		IDXGIFactory2* dxgiFactory2 = nullptr;
		hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
		if (dxgiFactory2)
		{
			DXGI_SWAP_CHAIN_DESC1 sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.Width = width;
			sd.Height = height;
			sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.BufferCount = 1;

			hr = dxgiFactory2->CreateSwapChainForHwnd(owner->g_pd3dDevice, hWnd, &sd, nullptr, nullptr, &g_pSwapChain1);
			if (SUCCEEDED(hr))
			{
				hr = g_pSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&g_pSwapChain));
			}

			dxgiFactory2->Release();
		}
		else
		{
			// DirectX 11.0 systems
			DXGI_SWAP_CHAIN_DESC sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.BufferCount = 1;
			sd.BufferDesc.Width = width;
			sd.BufferDesc.Height = height;
			sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sd.BufferDesc.RefreshRate.Numerator = 60;
			sd.BufferDesc.RefreshRate.Denominator = 1;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.OutputWindow = hWnd;
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.Windowed = TRUE;

			hr = dxgiFactory->CreateSwapChain(owner->g_pd3dDevice, &sd, &g_pSwapChain);
		}
		// Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
		dxgiFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);
		dxgiFactory->Release();
		if (FAILED(hr)) return;// hr;
		// Create a render target view		
		hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&g_pBackBuffer));
		if (FAILED(hr)) return;// hr;
		hr = owner->g_pd3dDevice->CreateRenderTargetView(g_pBackBuffer, nullptr, &g_pRenderTargetView);
		g_pBackBuffer->Release();
		if (FAILED(hr)) return;// hr;

		//Create textures for radar
		ID3D11Texture2D* pTexture;
		ID3D11RenderTargetView* pRenderTargetView;
		ID3D11ShaderResourceView* pShaderResourceView;
		D3D11_TEXTURE2D_DESC textureDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;

		// ---> Create texture for final 8bit layer
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		textureDesc.Width = extensionCoef * width;
		textureDesc.Height = extensionCoef * height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		hr = owner->g_pd3dDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);
		if (FAILED(hr)) return;// hr;
		ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		hr = owner->g_pd3dDevice->CreateRenderTargetView(pTexture, &renderTargetViewDesc, &pRenderTargetView);
		if (FAILED(hr)) return;// hr;
		ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		hr = owner->g_pd3dDevice->CreateShaderResourceView(pTexture, &shaderResourceViewDesc, &pShaderResourceView);
		if (FAILED(hr)) return;// hr;

		textureFinal8bit = RLGM::Texture{ pTexture, pRenderTargetView, pShaderResourceView };
		// <--- Create texture for final 8bit layer

		// ---> Create texture for final 2bit layer
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		textureDesc.Width = extensionCoef * width;
		textureDesc.Height = extensionCoef * height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		hr = owner->g_pd3dDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);
		if (FAILED(hr)) return;// hr;
		ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		hr = owner->g_pd3dDevice->CreateRenderTargetView(pTexture, &renderTargetViewDesc, &pRenderTargetView);
		if (FAILED(hr)) return;// hr;
		ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		hr = owner->g_pd3dDevice->CreateShaderResourceView(pTexture, &shaderResourceViewDesc, &pShaderResourceView);
		if (FAILED(hr)) return;// hr;

		textureFinal2bit = RLGM::Texture{ pTexture, pRenderTargetView, pShaderResourceView };
		// <--- Create texture for final 2bit layer

		// ---> Create texture for final tail layer
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		textureDesc.Width = extensionCoef * width;
		textureDesc.Height = extensionCoef * height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		hr = owner->g_pd3dDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);
		if (FAILED(hr)) return;// hr;
		ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		hr = owner->g_pd3dDevice->CreateRenderTargetView(pTexture, &renderTargetViewDesc, &pRenderTargetView);
		if (FAILED(hr)) return;// hr;
		ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		hr = owner->g_pd3dDevice->CreateShaderResourceView(pTexture, &shaderResourceViewDesc, &pShaderResourceView);
		if (FAILED(hr)) return;// hr;

		textureFinalTail = RLGM::Texture{ pTexture, pRenderTargetView, pShaderResourceView };
		// <--- Create texture for final tail layer
		
		// ---> Create texture for final layer without color correction
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		textureDesc.Width = extensionCoef * width;
		textureDesc.Height = extensionCoef * height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		hr = owner->g_pd3dDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);
		if (FAILED(hr)) return;// hr;
		ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		hr = owner->g_pd3dDevice->CreateRenderTargetView(pTexture, &renderTargetViewDesc, &pRenderTargetView);
		if (FAILED(hr)) return;// hr;
		ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		hr = owner->g_pd3dDevice->CreateShaderResourceView(pTexture, &shaderResourceViewDesc, &pShaderResourceView);
		if (FAILED(hr)) return;// hr;

		textureFinalWithoutColorCorrection = RLGM::Texture{ pTexture, pRenderTargetView, pShaderResourceView };
		
		hr = owner->g_pd3dDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);
		if (FAILED(hr)) return;// hr;
		hr = owner->g_pd3dDevice->CreateRenderTargetView(pTexture, &renderTargetViewDesc, &pRenderTargetView);
		if (FAILED(hr)) return;// hr;
		hr = owner->g_pd3dDevice->CreateShaderResourceView(pTexture, &shaderResourceViewDesc, &pShaderResourceView);
		if (FAILED(hr)) return;// hr;
		textureFinalWithoutColorCorrectionHelp1 = RLGM::Texture{ pTexture, pRenderTargetView, pShaderResourceView };
		hr = owner->g_pd3dDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);
		if (FAILED(hr)) return;// hr;
		hr = owner->g_pd3dDevice->CreateRenderTargetView(pTexture, &renderTargetViewDesc, &pRenderTargetView);
		if (FAILED(hr)) return;// hr;
		hr = owner->g_pd3dDevice->CreateShaderResourceView(pTexture, &shaderResourceViewDesc, &pShaderResourceView);
		if (FAILED(hr)) return;// hr;
		textureFinalWithoutColorCorrectionHelp2 = RLGM::Texture{ pTexture, pRenderTargetView, pShaderResourceView };
		//<--- Create texture for final layer without color correction
		
		//---> Create 8bit color correction texture for radar
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		textureDesc.Width = 256; //???
		textureDesc.Height = 256;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		hr = owner->g_pd3dDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);
		if (FAILED(hr)) return;// hr;
		ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;
		hr = owner->g_pd3dDevice->CreateShaderResourceView(pTexture, &shaderResourceViewDesc, &pShaderResourceView);
		if (FAILED(hr)) return;// hr;

		colorCorrection8bitTexture = RLGM::Texture{ pTexture, nullptr, pShaderResourceView };
		//<--- Create 8bit color correction texture for radar

		//---> Create 2bit color correction texture for radar
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		textureDesc.Width = 256; //???
		textureDesc.Height = 256;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		hr = owner->g_pd3dDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);
		if (FAILED(hr)) return;// hr;
		ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;
		hr = owner->g_pd3dDevice->CreateShaderResourceView(pTexture, &shaderResourceViewDesc, &pShaderResourceView);
		if (FAILED(hr)) return;// hr;

		colorCorrection2bitTexture = RLGM::Texture{ pTexture, nullptr, pShaderResourceView };
		//<--- Create 2bit color correction texture for radar
		
		//---> Create tail color correction texture for radar
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		textureDesc.Width = 256; //???
		textureDesc.Height = 256;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		hr = owner->g_pd3dDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);
		if (FAILED(hr)) return;// hr;
		ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;
		hr = owner->g_pd3dDevice->CreateShaderResourceView(pTexture, &shaderResourceViewDesc, &pShaderResourceView);
		if (FAILED(hr)) return;// hr;

		colorCorrectionTailTexture = RLGM::Texture{ pTexture, nullptr, pShaderResourceView };
		//<--- Create tail color correction texture for radar


		//---> Create Map layer
		for (int i = 0; i < 2; i++) {
			ZeroMemory(&textureDesc, sizeof(textureDesc));
			textureDesc.Width = width;
			textureDesc.Height = height;
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 1;
			textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GDI_COMPATIBLE;
			hr = owner->g_pd3dDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);
			if (FAILED(hr)) return;// hr;
			ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
			shaderResourceViewDesc.Format = textureDesc.Format;
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
			shaderResourceViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;
			hr = owner->g_pd3dDevice->CreateShaderResourceView(pTexture, &shaderResourceViewDesc, &pShaderResourceView);
			if (FAILED(hr)) return;// hr;

			layerMap[i] = RLGM::Texture{ pTexture, nullptr, pShaderResourceView };
		}
		//<--- Create Map layer
	}

	void View::AddRadar(int nRadarId)
	{
		show8bit[nRadarId] = true;
		show2bit[nRadarId] = true;
		showTails[nRadarId] = true;
		geoms[nRadarId] = RLGM::RadarPositioning{ 0,0,1,0 };

		//Create textures for radar
		ID3D11Texture2D* pTexture;
		ID3D11RenderTargetView* pRenderTargetView;
		ID3D11ShaderResourceView* pShaderResourceView;
		D3D11_TEXTURE2D_DESC textureDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		HRESULT hr;

		// ---> Create texture for drawing each radar on its own layer
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		textureDesc.Width = extensionCoef * width;
		textureDesc.Height = extensionCoef * height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;				
		hr = owner->g_pd3dDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);
		if (FAILED(hr)) return;// hr;
		ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		hr = owner->g_pd3dDevice->CreateRenderTargetView(pTexture, &renderTargetViewDesc, &pRenderTargetView);
		if (FAILED(hr)) return;// hr;
		ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		hr = owner->g_pd3dDevice->CreateShaderResourceView(pTexture, &shaderResourceViewDesc, &pShaderResourceView);
		if (FAILED(hr)) return;// hr;
		
		textureRadarCurrent.push_back({ pTexture, pRenderTargetView, pShaderResourceView });
		// <--- Create texture for drawing each radar on its own layer
	}
	void View::RemoveRadar(int id) 
	{
		show8bit.erase(id);
		geoms.erase(id);
	}

	//позиционирование Радара: центр, масштаб, поворот
	void View::SetRadarPosition(int id, int nX, int nY) 
	{
		float x = 2 * nX / (float)width - 1;
		float y = 1 - 2 * nY / (float)height;

		for (auto& var : geoms)
			if ((var.first == id) || (id == -1)) {
				geoms[var.first].centerX = x;
				geoms[var.first].centerY = y;
			}
	}
	std::pair<int, int> View::GetRadarPosition(int id) 
	{ 
		return std::pair<int, int> { (int)((geoms[id].centerX + 1) * width / 2), (int)((1 - geoms[id].centerY) * height / 2) };
	}
	void View::AddRadarPosition(int id, int nDX, int nDY) 
	{
		float dx = 2 * nDX / (float)width;
		float dy = -2 * nDY / (float)height;

		for (auto& var : geoms)
			if ((var.first == id) || (id == -1)) {
				geoms[var.first].centerX += dx;
				geoms[var.first].centerY += dy;
			}
	}
	void View::SetDPP(int id, float dDPP) 
	{
		for (auto& var : geoms)
			if ((var.first == id) || (id == -1))
				geoms[var.first].radius = (2 * owner->radars[var.first].numSamples / dDPP) / height;
	}
	float View::GetDPP(int id) 
	{ 
		return (2 * owner->radars[id].numSamples) / (geoms[id].radius * height);
	}
	void View::MultiplyDPP(int id, float value) 
	{
		for (auto& var : geoms)
			if ((var.first == id) || (id == -1))
				geoms[var.first].radius *= value;
	}
	void View::MultiplyDPPCenter(int id, float value) 
	{
		for (auto& var : geoms)
			if ((var.first == id) || (id == -1)) {
				geoms[var.first].radius *= value;
				geoms[var.first].centerX *= value;
				geoms[var.first].centerY *= value;
			}
	}
	void View::SetRotation(int id, float rot) 
	{
		for (auto& var : geoms)
			if ((var.first == id) || (id == -1))
				geoms[var.first].rotation = rot;
	}
	void View::AddRotation(int id, float rot) 
	{
		for (auto& var : geoms)
			if ((var.first == id) || (id == -1))
				geoms[var.first].rotation += rot;
	}
	
	void View::SetShow8bit(int nRadarId, bool value)
	{
		for (auto& var : show8bit)
			if ((var.first == nRadarId) || (nRadarId == -1))
				show8bit[var.first] = value;
	}
	bool View::GetShow8bit(int nRadarId) {
		return show8bit[nRadarId];
	}
	void View::SetShow2bit(int nRadarId, bool value)
	{
		for (auto& var : show2bit)
			if ((var.first == nRadarId) || (nRadarId == -1))
				show2bit[var.first] = value;
	}
	bool View::GetShow2bit(int nRadarId) {
		return show2bit[nRadarId];
	}
	void View::SetShowTails(int nRadarId, bool value)
	{
		for (auto& var : showTails)
			if ((var.first == nRadarId) || (nRadarId == -1))
				showTails[var.first] = value;
	}
	bool View::GetShowTails(int nRadarId) {
		return showTails[nRadarId];
	}

	void View::SetColorCorrection8bit(int nRadarId, int nCount, uint32_t* value) {
		D3D11_BOX region;
		region.left = 0;
		region.right = 256;//???
		region.top = nRadarId;
		region.bottom = nRadarId + 1;
		region.front = 0;
		region.back = 1;
		owner->g_pImmediateContext->UpdateSubresource(colorCorrection8bitTexture.m_Texture, 0, &region, value, 256, 256);
	}
	void View::SetColorCorrection2bit(int nRadarId, int nCount, uint32_t* value) {
		D3D11_BOX region;
		region.left = 0;
		region.right = 256;//???
		region.top = nRadarId;
		region.bottom = nRadarId + 1;
		region.front = 0;
		region.back = 1;
		owner->g_pImmediateContext->UpdateSubresource(colorCorrection2bitTexture.m_Texture, 0, &region, value, 256, 256);
	}
	void View::SetColorCorrectionTails(int nRadarId, int nCount, uint32_t* value) {
		D3D11_BOX region;
		region.left = 0;
		region.right = 256;//???
		region.top = nRadarId;
		region.bottom = nRadarId + 1;
		region.front = 0;
		region.back = 1;
		owner->g_pImmediateContext->UpdateSubresource(colorCorrectionTailTexture.m_Texture, 0, &region, value, 256, 256);
	}


	//отрисовать Окно
	RLGM::ReturnCode View::Draw()
	{		
		ID3D11ShaderResourceView* pNullSRV = NULL;
		UINT stride = sizeof(RLGM::PositionedTexturedVertex);
		UINT offset = 0;
		RLGM::ConstantBuffer1 cb1;
		RLGM::ConstantBuffer4 cb4;
		cb1.mViewWorldProjection = DirectX::XMMatrixIdentity();
		float black[4]{ 0,0,0,0 };
		

		// Disable alpha blending
		UINT sampleMask = 0xffffffff;
		owner->g_pImmediateContext->OMSetBlendState(owner->g_pBlendStateNoBlend, nullptr, sampleMask);
		// Set the viewport
		D3D11_VIEWPORT vp1{ 0, 0, extensionCoef * width, extensionCoef * height, 0, 1 };
		owner->g_pImmediateContext->RSSetViewports(1, &vp1);

		//---> Forming final 2bit layer (3 steps)
		//if (draw2bit) //should be drawn because unified 2bit info is used for tails unification
		{
			int num_radars = 0; //number of actually drawn 2bits
			
			//---> 1. Drawing 2bit of each radar into separate layers
			for (auto& var : owner->radars)
			{
				int radar_id = var.first;
				if (!show2bit[radar_id]) continue;
				owner->g_pImmediateContext->OMSetRenderTargets(1, &textureRadarCurrent[num_radars].m_renderTargetView, nullptr);				
				owner->g_pImmediateContext->ClearRenderTargetView(textureRadarCurrent[num_radars].m_renderTargetView, black);
				owner->radars[radar_id].Draw2bit(geoms[radar_id], aspectRatio, owner->psShaders[L"shader2bit.cso"], owner->vsShaders[L"vs_shader1.cso"], owner->samplerPoint/*, radarDrawMode*/);
				num_radars++;
			}
			//<--- Drawing 2bit of each radar into separate layers

			//---> 2. Unification of 2bit layers
			owner->g_pImmediateContext->OMSetRenderTargets(1, &textureFinalWithoutColorCorrectionHelp1.m_renderTargetView, nullptr);
			owner->g_pImmediateContext->PSSetShader(owner->psShaders[L"shader2bit_unify.cso"], nullptr, 0);
			cb4 = RLGM::ConstantBuffer4{ num_radars, (int)unification2bitRule };
			owner->g_pImmediateContext->UpdateSubresource(owner->constantBuffer4, 0, nullptr, &cb4, 0, 0);
			owner->g_pImmediateContext->PSSetConstantBuffers(0, 1, &owner->constantBuffer4);
			owner->g_pImmediateContext->PSSetSamplers(0, 1, &owner->samplerPoint);
			for (int k = 0; k < num_radars; k++)
				owner->g_pImmediateContext->PSSetShaderResources(k, 1, &textureRadarCurrent[k].m_shaderResourceView);
			owner->g_pImmediateContext->VSSetShader(owner->vsShaders[L"vs_shader1.cso"], nullptr, 0);
			owner->g_pImmediateContext->UpdateSubresource(owner->constantBuffer1, 0, nullptr, &cb1, 0, 0);
			owner->g_pImmediateContext->VSSetConstantBuffers(0, 1, &owner->constantBuffer1);
			owner->g_pImmediateContext->IASetVertexBuffers(0, 1, &owner->vbQuadScreen, &stride, &offset);
			owner->g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			owner->g_pImmediateContext->Draw(6, 0);
			for (int k = 0; k < num_radars; k++) owner->g_pImmediateContext->PSSetShaderResources(k, 1, &pNullSRV);
			//<--- 2. Unification of 2bit layers

			//---> 3. Applying color correction
			owner->g_pImmediateContext->OMSetRenderTargets(1, &textureFinal2bit.m_renderTargetView, nullptr);
			owner->g_pImmediateContext->PSSetShader(owner->psShaders[L"shader2bit_colorcorrection.cso"], nullptr, 0);
			owner->g_pImmediateContext->PSSetShaderResources(0, 1, &textureFinalWithoutColorCorrectionHelp1.m_shaderResourceView);
			owner->g_pImmediateContext->PSSetSamplers(0, 1, &owner->samplerPoint);
			owner->g_pImmediateContext->PSSetShaderResources(1, 1, &colorCorrection2bitTexture.m_shaderResourceView);
			owner->g_pImmediateContext->IASetVertexBuffers(0, 1, &owner->vbQuadScreen, &stride, &offset);
			owner->g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			owner->g_pImmediateContext->Draw(6, 0);
			//<--- 3. Applying color correction
		}

		//---> Forming final Tails layer (5 steps)
		if (drawTails) 
		{
			//---> 1. Drawing tail 2bit of each radar into separate layers
			int num_radars = 0; //number of actually drawn tail 2bits
			for (auto& var : owner->radars)
			{
				int radar_id = var.first;
				if (!showTails[radar_id]) continue;
				owner->g_pImmediateContext->OMSetRenderTargets(1, &textureRadarCurrent[num_radars].m_renderTargetView, nullptr);
				owner->g_pImmediateContext->ClearRenderTargetView(textureRadarCurrent[num_radars].m_renderTargetView, black);
				owner->radars[radar_id].Draw2bitTail(geoms[radar_id], aspectRatio, owner->psShaders[L"shader2bit.cso"], owner->vsShaders[L"vs_shader1.cso"], owner->samplerPoint/*, radarDrawMode*/);
				num_radars++;
			}
			//<--- 1. Drawing tail 2bit of each radar into separate layers

			//---> 2. Unification of tail 2bit layers
			owner->g_pImmediateContext->OMSetRenderTargets(1, &textureFinalWithoutColorCorrectionHelp2.m_renderTargetView, nullptr);
			owner->g_pImmediateContext->PSSetShader(owner->psShaders[L"shader2bit_unify.cso"], nullptr, 0);
			cb4 = RLGM::ConstantBuffer4{ num_radars, (int)unification2bitRule };
			owner->g_pImmediateContext->UpdateSubresource(owner->constantBuffer4, 0, nullptr, &cb4, 0, 0);
			owner->g_pImmediateContext->PSSetConstantBuffers(0, 1, &owner->constantBuffer4);
			owner->g_pImmediateContext->PSSetSamplers(0, 1, &owner->samplerPoint);
			for (int k = 0; k < num_radars; k++)
				owner->g_pImmediateContext->PSSetShaderResources(k, 1, &textureRadarCurrent[k].m_shaderResourceView);
			owner->g_pImmediateContext->VSSetShader(owner->vsShaders[L"vs_shader1.cso"], nullptr, 0);
			owner->g_pImmediateContext->UpdateSubresource(owner->constantBuffer1, 0, nullptr, &cb1, 0, 0);
			owner->g_pImmediateContext->VSSetConstantBuffers(0, 1, &owner->constantBuffer1);
			owner->g_pImmediateContext->IASetVertexBuffers(0, 1, &owner->vbQuadScreen, &stride, &offset);
			owner->g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			owner->g_pImmediateContext->Draw(6, 0);			
			for (int k = 0; k < num_radars; k++) owner->g_pImmediateContext->PSSetShaderResources(k, 1, &pNullSRV);
			//<--- 2. Unification of tail 2bit layers

			//---> 3. Drawing tail of each radar into separate layers
			num_radars = 0; //number of actually drawn tails
			for (auto& var : owner->radars)
			{
				int radar_id = var.first;
				if (!showTails[radar_id]) continue;
				owner->g_pImmediateContext->OMSetRenderTargets(1, &textureRadarCurrent[num_radars].m_renderTargetView, nullptr);
				owner->g_pImmediateContext->ClearRenderTargetView(textureRadarCurrent[num_radars].m_renderTargetView, black);
				owner->radars[radar_id].DrawTail(geoms[radar_id], aspectRatio, owner->psShaders[L"shaderTail.cso"], owner->vsShaders[L"vs_shader1.cso"], owner->samplerPoint);//, radarDrawMode);
				num_radars++;
			}
			//<--- 3. Drawing tail of each radar into separate layers

			//---> 4. Unification of tail layers
			owner->g_pImmediateContext->OMSetRenderTargets(1, &textureFinalWithoutColorCorrection.m_renderTargetView, nullptr);
			owner->g_pImmediateContext->PSSetShader(owner->psShaders[L"shaderTail_unify.cso"], nullptr, 0);
			cb4 = RLGM::ConstantBuffer4{ num_radars, owner->thr2bitForTails };
			owner->g_pImmediateContext->UpdateSubresource(owner->constantBuffer4, 0, nullptr, &cb4, 0, 0);
			owner->g_pImmediateContext->PSSetConstantBuffers(0, 1, &owner->constantBuffer4);
			owner->g_pImmediateContext->PSSetSamplers(0, 1, &owner->samplerPoint);
			owner->g_pImmediateContext->PSSetShaderResources(0, 1, &textureFinalWithoutColorCorrectionHelp2.m_shaderResourceView);
			owner->g_pImmediateContext->PSSetShaderResources(1, 1, &textureFinalWithoutColorCorrectionHelp1.m_shaderResourceView);
			for (int k = 0; k < num_radars; k++)
				owner->g_pImmediateContext->PSSetShaderResources(k + 2, 1, &textureRadarCurrent[k].m_shaderResourceView);
			owner->g_pImmediateContext->VSSetShader(owner->vsShaders[L"vs_shader1.cso"], nullptr, 0);
			cb1.mViewWorldProjection = DirectX::XMMatrixIdentity();
			owner->g_pImmediateContext->UpdateSubresource(owner->constantBuffer1, 0, nullptr, &cb1, 0, 0);
			owner->g_pImmediateContext->VSSetConstantBuffers(0, 1, &owner->constantBuffer1);
			stride = sizeof(RLGM::PositionedTexturedVertex);
			offset = 0;
			owner->g_pImmediateContext->IASetVertexBuffers(0, 1, &owner->vbQuadScreen, &stride, &offset);
			owner->g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			owner->g_pImmediateContext->Draw(6, 0);
			for (int k = 0; k < num_radars; k++) owner->g_pImmediateContext->PSSetShaderResources(k + 2, 1, &pNullSRV);
			//<--- 4. Unification of tail layers

			//---> 5. Applying color correction
			owner->g_pImmediateContext->OMSetRenderTargets(1, &textureFinalTail.m_renderTargetView, nullptr);
			owner->g_pImmediateContext->PSSetShader(owner->psShaders[L"shaderTail_colorcorrection.cso"], nullptr, 0);
			owner->g_pImmediateContext->PSSetShaderResources(0, 1, &textureFinalWithoutColorCorrection.m_shaderResourceView);
			owner->g_pImmediateContext->PSSetSamplers(0, 1, &owner->samplerPoint);
			owner->g_pImmediateContext->PSSetShaderResources(1, 1, &colorCorrectionTailTexture.m_shaderResourceView);
			owner->g_pImmediateContext->IASetVertexBuffers(0, 1, &owner->vbQuadScreen, &stride, &offset);
			owner->g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			owner->g_pImmediateContext->Draw(6, 0);
			//<--- 5. Applying color correction
		}
		
		//---> Forming final 8bit layer (3 steps)
		if (draw8bit)
		{
			int num_radars = 0; //number of actually drawn 8bits

			//---> 1. Drawing 8bit of each radar into separate layers
			for (auto& var : owner->radars)
			{
				int radar_id = var.first;
				if (!show8bit[radar_id]) continue;
				owner->g_pImmediateContext->OMSetRenderTargets(1, &textureRadarCurrent[num_radars].m_renderTargetView, nullptr);
				owner->g_pImmediateContext->ClearRenderTargetView(textureRadarCurrent[num_radars].m_renderTargetView, black);
				ID3D11SamplerState* txSampler = (m_8bitTexture_SamplerType == RLGM::TextureSamplerType::POINT) ? owner->samplerPoint : owner->samplerLinear;
				owner->radars[radar_id].Draw8bit(geoms[radar_id], aspectRatio, owner->psShaders[L"shader8bit.cso"], owner->vsShaders[L"vs_shader1.cso"], txSampler/*, radarDrawMode*/);
				num_radars++;
			}
			//<--- 1. Drawing 8bit of each radar into separate layers

			//---> 2. Unification of 8bit layers
			owner->g_pImmediateContext->OMSetRenderTargets(1, &textureFinalWithoutColorCorrection.m_renderTargetView, nullptr);
			owner->g_pImmediateContext->PSSetShader(owner->psShaders[L"shader8bit_unify.cso"], nullptr, 0);
			cb4 = RLGM::ConstantBuffer4{ num_radars, (int)unification8bitRule };
			owner->g_pImmediateContext->UpdateSubresource(owner->constantBuffer4, 0, nullptr, &cb4, 0, 0);
			owner->g_pImmediateContext->PSSetConstantBuffers(0, 1, &owner->constantBuffer4);
			owner->g_pImmediateContext->PSSetSamplers(0, 1, &owner->samplerPoint);
			for (int k = 0; k < num_radars; k++)
				owner->g_pImmediateContext->PSSetShaderResources(k, 1, &textureRadarCurrent[k].m_shaderResourceView);
			owner->g_pImmediateContext->VSSetShader(owner->vsShaders[L"vs_shader1.cso"], nullptr, 0);
			owner->g_pImmediateContext->UpdateSubresource(owner->constantBuffer1, 0, nullptr, &cb1, 0, 0);
			owner->g_pImmediateContext->VSSetConstantBuffers(0, 1, &owner->constantBuffer1);
			owner->g_pImmediateContext->IASetVertexBuffers(0, 1, &owner->vbQuadScreen, &stride, &offset);
			owner->g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			owner->g_pImmediateContext->Draw(6, 0);
			for (int k = 0; k < num_radars; k++) owner->g_pImmediateContext->PSSetShaderResources(k, 1, &pNullSRV);
			//<--- 2. Unification of 8bit layers

			//---> 3. Applying color correction
			owner->g_pImmediateContext->OMSetRenderTargets(1, &textureFinal8bit.m_renderTargetView, nullptr);
			owner->g_pImmediateContext->PSSetShader(owner->psShaders[L"shader8bit_colorcorrection.cso"], nullptr, 0);
			owner->g_pImmediateContext->PSSetShaderResources(0, 1, &textureFinalWithoutColorCorrection.m_shaderResourceView);
			owner->g_pImmediateContext->PSSetSamplers(0, 1, &owner->samplerPoint);
			owner->g_pImmediateContext->PSSetShaderResources(1, 1, &colorCorrection8bitTexture.m_shaderResourceView);
			owner->g_pImmediateContext->IASetVertexBuffers(0, 1, &owner->vbQuadScreen, &stride, &offset);
			owner->g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			owner->g_pImmediateContext->Draw(6, 0);
			//<--- 3. Applying color correction
		}

		// Clear the back buffer
		owner->g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);
		owner->g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, DirectX::Colors::Black);
		// Set the viewport
		D3D11_VIEWPORT vp2{ 0, 0, width, height, 0, 1 };
		owner->g_pImmediateContext->RSSetViewports(1, &vp2);
		
		if (drawMap)
		{			
			owner->g_pImmediateContext->PSSetShader(owner->psShaders[L"shaderMap.cso"], nullptr, 0);
			owner->g_pImmediateContext->PSSetShaderResources(0, 1, &layerMap[layerMapIndexToDraw].m_shaderResourceView);
			owner->g_pImmediateContext->PSSetSamplers(0, 1, &owner->samplerPoint);
			RLGM::ConstantBuffer1 cb1;
			cb1.mViewWorldProjection = DirectX::XMMatrixRotationZ(-mapGeom.rotation * ((float)RLGM::M_PI / 180)) *
				DirectX::XMMatrixScaling(mapGeom.radius, mapGeom.radius, 1) *
				DirectX::XMMatrixTranslation(mapGeom.centerX, mapGeom.centerY, 0);
			owner->g_pImmediateContext->UpdateSubresource(owner->constantBuffer1, 0, nullptr, &cb1, 0, 0);
			owner->g_pImmediateContext->IASetVertexBuffers(0, 1, &owner->vbQuadScreen, &stride, &offset);
			owner->g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			owner->g_pImmediateContext->Draw(6, 0);

		}		

		if (draw8bit)
		{
			// enable alpha blending if map is drawn
			if (drawMap) 
			{	
				sampleMask = 0xffffffff;
				owner->g_pImmediateContext->OMSetBlendState(owner->g_pBlendStateBlend, nullptr, sampleMask);
			}
			owner->g_pImmediateContext->PSSetShader(owner->psShaders[L"shader8bit_final.cso"], nullptr, 0);
			owner->g_pImmediateContext->PSSetShaderResources(0, 1, &textureFinal8bit.m_shaderResourceView);
			owner->g_pImmediateContext->PSSetSamplers(0, 1, &owner->samplerPoint);
			owner->g_pImmediateContext->VSSetShader(owner->vsShaders[L"vs_shader1.cso"], nullptr, 0);
			owner->g_pImmediateContext->UpdateSubresource(owner->constantBuffer1, 0, nullptr, &cb1, 0, 0);
			owner->g_pImmediateContext->VSSetConstantBuffers(0, 1, &owner->constantBuffer1);
			owner->g_pImmediateContext->IASetVertexBuffers(0, 1, &owner->vbQuadScreen, &stride, &offset);
			owner->g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			owner->g_pImmediateContext->Draw(6, 0);
		}

		// enable alpha blending
		sampleMask = 0xffffffff;
		owner->g_pImmediateContext->OMSetBlendState(owner->g_pBlendStateBlend, nullptr, sampleMask);

		if (drawTails)
		{
			owner->g_pImmediateContext->PSSetShader(owner->psShaders.at(L"shaderTail_final.cso"), nullptr, 0);
			owner->g_pImmediateContext->PSSetShaderResources(0, 1, &textureFinalTail.m_shaderResourceView);
			owner->g_pImmediateContext->PSSetSamplers(0, 1, &owner->samplerPoint);
			owner->g_pImmediateContext->VSSetShader(owner->vsShaders.at(L"vs_shader1.cso"), nullptr, 0);
			owner->g_pImmediateContext->UpdateSubresource(owner->constantBuffer1, 0, nullptr, &cb1, 0, 0);
			owner->g_pImmediateContext->VSSetConstantBuffers(0, 1, &owner->constantBuffer1);
			owner->g_pImmediateContext->IASetVertexBuffers(0, 1, &owner->vbQuadScreen, &stride, &offset);
			owner->g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			owner->g_pImmediateContext->Draw(6, 0);

		}

		if (draw2bit)
		{
			owner->g_pImmediateContext->PSSetShader(owner->psShaders[L"shader2bit_final.cso"], nullptr, 0);
			owner->g_pImmediateContext->PSSetShaderResources(0, 1, &textureFinal2bit.m_shaderResourceView);
			owner->g_pImmediateContext->PSSetSamplers(0, 1, &owner->samplerPoint);
			owner->g_pImmediateContext->VSSetShader(owner->vsShaders[L"vs_shader1.cso"], nullptr, 0);
			owner->g_pImmediateContext->UpdateSubresource(owner->constantBuffer1, 0, nullptr, &cb1, 0, 0);
			owner->g_pImmediateContext->VSSetConstantBuffers(0, 1, &owner->constantBuffer1);
			owner->g_pImmediateContext->IASetVertexBuffers(0, 1, &owner->vbQuadScreen, &stride, &offset);
			owner->g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			owner->g_pImmediateContext->Draw(6, 0);
		}

		// Present the information rendered to the back buffer to the front buffer (the screen)
		g_pSwapChain->Present(0, 0);

		return RLGM::ReturnCode::Success;
	}

	//возврат Картографического слоя
	ID3D11Texture2D* View::GetMapLayer() {
		return layerMap[1 - layerMapIndexToDraw].m_Texture;
	}
	void View::ReleaseMapLayer() {
		layerMapIndexToDraw = 1 - layerMapIndexToDraw;
	}

	//позиционирование нерадарных слоёв
	void View::SetSurfacesShift(int nx, int ny) {
		float x = 2 * nx / (float)width - 1;
		float y = 1 - 2 * ny / (float)height;
		mapGeom.centerX = x;
		mapGeom.centerY = y;
	}
	void View::AddSurfacesShift(int nDX, int nDY) {
		float dx = 2 * nDX / (float)width;
		float dy = -2 * nDY / (float)height;
		mapGeom.centerX += dx;
		mapGeom.centerY += dy;
	}
	void View::MultiplySurfacesDPP(float value) {
		mapGeom.radius *= value;
	}
	void View::MultiplySurfacesDPPCenter(float value) {
		mapGeom.radius *= value;
		mapGeom.centerX *= value;
		mapGeom.centerY *= value;
	}

	View::~View()
	{
	}
};
#include "stdafx.h"
#include "Interface.h"

namespace RLGM
{
	RLGM::ReturnCode Interface::CreateDevice()
	{
		/*IDXGIFactory * pFactory = NULL;

		CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
		
		UINT i = 0;
		IDXGIAdapter * pAdapter;
		std::vector <IDXGIAdapter*> vAdapters;
		while (pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND)
		{
			vAdapters.push_back(pAdapter);
			++i;
			
			auto adapterDescription = DXGI_ADAPTER_DESC();
			pAdapter->GetDesc(&adapterDescription);
			std::cout << adapterDescription.Description << std::endl;
		}*/

		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);

		if (FAILED(hr)) return RLGM::ReturnCode::D3DDeviceCreationError;

		D3D11_RASTERIZER_DESC rasterDesc;
		ZeroMemory(&rasterDesc, sizeof(rasterDesc));
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_NONE;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = true;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		hr = g_pd3dDevice->CreateRasterizerState(&rasterDesc, &g_rasterizerState);
		if (FAILED(hr)) return RLGM::ReturnCode::D3DDeviceCreationError;
		g_pImmediateContext->RSSetState(g_rasterizerState);

		// from DX11 samples
		/*IDXGIFactory1* dxgiFactory = nullptr;
		{
			IDXGIDevice* dxgiDevice = nullptr;
			hr = g_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
			if (SUCCEEDED(hr))
			{
				IDXGIAdapter* adapter = nullptr;
				hr = dxgiDevice->GetAdapter(&adapter);
				if (SUCCEEDED(hr))
				{
					hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
					adapter->Release();
				}
				dxgiDevice->Release();
			}
		}

		if (FAILED(hr)) return RLGM::ReturnCode::D3DDeviceCreationError;

		IDXGIFactory2* dxgiFactory2 = nullptr;
		hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
		if (dxgiFactory2)
		{
			// DirectX 11.1 or later
			hr = g_pd3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&g_pd3dDevice1));
			if (SUCCEEDED(hr))
			{
				(void)g_pImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&g_pImmediateContext1));
			}
		}*/

		return RLGM::ReturnCode::Success;
	}

	HRESULT Interface::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
	{
		HRESULT hr = S_OK;

		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;// D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY;// D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
		dwShaderFlags |= D3DCOMPILE_DEBUG;

		// Disable optimizations to further improve shader debugging
		dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
		//D3DCOMPILE_PREFER_FLOW_CONTROL | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION
#endif

		ID3DBlob* pErrorBlob = nullptr;
		hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
			dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
		if (FAILED(hr))
		{
			if (pErrorBlob)
			{
				OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
				pErrorBlob->Release();
			}
			return hr;
		}
		if (pErrorBlob) pErrorBlob->Release();

		return S_OK;
	}

	RLGM::ReturnCode Interface::LoadShaders()
	{
		std::wstring ps_shaders[] = { L"shader8bit.cso", L"shader8bit_colorcorrection.cso", L"shader8bit_final.cso", L"shader8bit_unify.cso",
									  L"shader2bit.cso", L"shader2bit_colorcorrection.cso", L"shader2bit_final.cso", L"shader2bit_unify.cso",
									  L"shaderTailCalc.cso", L"shaderTail.cso", L"shaderTail_unify.cso", L"shaderTail_colorcorrection.cso", L"shaderTail_final.cso",
									  L"shaderMap.cso" };

		std::wstring vs_shaders[] = { L"vs_shader1.cso" };
		
		for (auto& psShader : ps_shaders) {
			ID3D11PixelShader* PS;
			ID3DBlob* PS_Buffer;
			D3DReadFileToBlob(psShader.c_str(), &PS_Buffer);
			HRESULT hr = g_pd3dDevice->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);
			if (FAILED(hr)) 
				return RLGM::ReturnCode::ShaderLoadError;
			psShaders[psShader] = PS;
		}
		
		ID3D11VertexShader* VS;
		ID3DBlob* VS_Buffer;
		D3DReadFileToBlob(vs_shaders[0].c_str(), &VS_Buffer);
		HRESULT hr = g_pd3dDevice->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
		vsShaders[vs_shaders[0]] = VS;
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = ARRAYSIZE(layout);
		
		// Create the input layout
		hr = g_pd3dDevice->CreateInputLayout(layout, numElements, VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), &g_pVertexLayout);
		VS_Buffer->Release();
		if (FAILED(hr)) return RLGM::ReturnCode::ShaderLoadError;
	
		// Set the input layout
		g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

		//Pixel Shaders
		/*for (auto& psShader : ps_shaders)
		{
			// Compile the pixel shader
			ID3DBlob* pBlob = nullptr;
			HRESULT hr = CompileShaderFromFile((WCHAR*)psShader.c_str(), "PS", "ps_4_0", &pBlob);
			if (FAILED(hr)) {
				pBlob->Release();
				return RLGM::ReturnCode::ShaderLoadError;
			}

			// Create the pixel shader
			ID3D11PixelShader* pPixelShader;
			hr = g_pd3dDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
			pBlob->Release();
			if (FAILED(hr)) return RLGM::ReturnCode::ShaderLoadError;

			psShaders[psShader] = pPixelShader;
		}

		//Vertex Shaders
		for (auto& vsShader : vs_shaders)
		{
			// Compile the vertex shader
			ID3DBlob* pBlob = nullptr;
			HRESULT hr = CompileShaderFromFile((WCHAR*)vsShader.c_str(), "VS", "vs_4_0", &pBlob);
			if (FAILED(hr)) {
				pBlob->Release();
				return RLGM::ReturnCode::ShaderLoadError;
			}

			// Create the vertex shader
			ID3D11VertexShader* pVertexShader;
			hr = g_pd3dDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
			if (FAILED(hr)) {
				pBlob->Release();
				return RLGM::ReturnCode::ShaderLoadError;
			}

			vsShaders[vsShader] = pVertexShader;

			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			UINT numElements = ARRAYSIZE(layout);

			// Create the input layout
			hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &g_pVertexLayout);
			pBlob->Release();
			if (FAILED(hr)) return RLGM::ReturnCode::ShaderLoadError;

			// Set the input layout
			g_pImmediateContext->IASetInputLayout(g_pVertexLayout);
		}*/

		return RLGM::ReturnCode::Success;
	}

	RLGM::ReturnCode Interface::Init(int tails_calc_period_ms)
	{
		std::lock_guard<std::mutex> guard{ lock };

		RLGM::ReturnCode return_code;

		//создание Устройства рисования Direct3D
		return_code = CreateDevice();
		if (return_code != RLGM::ReturnCode::Success) return return_code;

		//загрузка шейдеров
		return_code = LoadShaders();
		if (return_code != RLGM::ReturnCode::Success) {
			std::cout << "Shader load error" << std::endl;
			return return_code;
		}

		//создание квада экрана
		RLGM::PositionedTexturedVertex quadScreenExtended[6];
		quadScreenExtended[0] = { DirectX::XMFLOAT3(-1, 1, 1), DirectX::XMFLOAT2(0, 0) };
		quadScreenExtended[1] = { DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT2(1, 0) };
		quadScreenExtended[2] = { DirectX::XMFLOAT3(-1, -1, 1), DirectX::XMFLOAT2(0, 1) };
		quadScreenExtended[3] = quadScreenExtended[1];
		quadScreenExtended[4] = quadScreenExtended[2];
		quadScreenExtended[5] = { DirectX::XMFLOAT3(1, -1, 1), DirectX::XMFLOAT2(1, 1) };
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = sizeof(RLGM::PositionedTexturedVertex) * 3 * 2;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = quadScreenExtended;
		HRESULT hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &vbQuadScreen);
		
		//create blenders
		D3D11_BLEND_DESC BlendState;
		ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
		BlendState.RenderTarget[0].BlendEnable = true;
		BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		hr = g_pd3dDevice->CreateBlendState(&BlendState, &g_pBlendStateBlend);
		if (FAILED(hr)) return RLGM::ReturnCode::Fail;// hr;
		BlendState.RenderTarget[0].BlendEnable = false;
		BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = g_pd3dDevice->CreateBlendState(&BlendState, &g_pBlendStateNoBlend);
		if (FAILED(hr)) return RLGM::ReturnCode::Fail;// hr;


		//create thread for Tails calculation
		tailsCalcPeriod_ms = tails_calc_period_ms;
		tailsCalcThread = std::thread(&Interface::CalcTails, this, tailsCalcPeriod_ms);

		// Create samplers
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &samplerLinear);
		if (FAILED(hr)) return RLGM::ReturnCode::Fail;// hr;
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		g_pd3dDevice->CreateSamplerState(&sampDesc, &samplerPoint);
		if (FAILED(hr)) return RLGM::ReturnCode::Fail;// hr;

		// Create the constant buffer
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(RLGM::ConstantBuffer1);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &constantBuffer1);
		if (FAILED(hr)) return RLGM::ReturnCode::Fail;// hr;
		
		bd.ByteWidth = sizeof(RLGM::ConstantBuffer3);
		hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &constantBuffer3);
		if (FAILED(hr)) return RLGM::ReturnCode::Fail;// hr;

		bd.ByteWidth = sizeof(RLGM::ConstantBuffer4);
		hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &constantBuffer4);
		if (FAILED(hr)) return RLGM::ReturnCode::Fail;// hr;

		//настройка Устройства рисования Direct3D
		/*return_code = TuneDevice();
		if (return_code != RLGM_ReturnCode.Success)
			return return_code;

		//создание Спрайта для рисования Картинок
		return_code = CreateSprite();
		if (return_code != RLGM_ReturnCode.Success)
			return return_code;*/

		//вернуть код успешного возврата
		return RLGM::ReturnCode::Success;
	}

	void Interface::CalcTails(int period_ms) {
		auto t_start = std::chrono::high_resolution_clock::now();
		std::chrono::milliseconds delay(period_ms);
		while (!m_bExit) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			auto t_now = std::chrono::high_resolution_clock::now();
			std::chrono::milliseconds elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t_now - t_start);
			if (delay <= elapsed) {
				t_start = t_now;
				
				std::lock_guard<std::mutex> guard{ lock };

				for (auto& key : radars) {
					radars.at(key.first).CalcTails(psShaders.at(L"shaderTailCalc.cso"), vsShaders.at(L"vs_shader1.cso"), 1);
					std::cout << "tails calc" << std::endl;
				}
			}
		}
	}

	void Interface::DeInit() {
		std::lock_guard<std::mutex> guard{ lock };
		m_bExit = true;
		tailsCalcThread.join();
	}

	//создать, удалить Окно
	RLGM::ReturnCode Interface::CreateView(HWND hWnd, int nViewId)
	{
		std::lock_guard<std::mutex> guard{ lock };

		//создать Окно и добавить его в словарь
		views.erase(nViewId);
		views.try_emplace(nViewId, hWnd, this);

		//добавить Радары этому Окну
		for (auto& var : radars)
			views[nViewId].AddRadar(var.first);

		return RLGM::ReturnCode::Success;
	}
	void Interface::DeleteView(int nViewId)
	{
		std::lock_guard<std::mutex> guard{ lock };
		views.erase(nViewId);
	}

	//создать, удалить Радар
	RLGM::ReturnCode Interface::CreateRadar(int nRadarId)
	{
		std::lock_guard<std::mutex> guard{ lock };

		radars.erase(nRadarId);

		//создать Радар и добавить его в словарь
		radars.try_emplace(nRadarId, nRadarId, this);

		//добавить Радар каждому Окну
		for (auto& var : views)
			views[var.first].AddRadar(nRadarId);

		return RLGM::ReturnCode::Success;
	}
	void Interface::DeleteRadar(int nRadarId)
	{
		std::lock_guard<std::mutex> guard{ lock };
		radars.erase(nRadarId);
	}

	//добавить ПРЛИ в Радар
	RLGM::ReturnCode Interface::AddPRLI(int nRadarId, RLGM::PRLIType nType, int nPRLISampleCount, int nPRLILineCount)
	{
		std::lock_guard<std::mutex> guard{ lock };

		for (auto& var : radars)
			if ((var.first == nRadarId) || (nRadarId == -1))
				radars[var.first].AddPRLI(nType, nPRLISampleCount, nPRLILineCount);
		return RLGM::ReturnCode::Success;
	}

	void Interface::SetShowPRLI(int nViewId, int nRadarId, RLGM::PRLIType nPRLIType, bool nShow) {
		for (auto& var : views)
		{
			if ((var.first == nViewId) || (nViewId == -1))
			{
				if (nPRLIType == RLGM::PRLIType::PRLI_Tail)
					views[var.first].SetShowTails(nRadarId, nShow);
				if (nPRLIType == RLGM::PRLIType::PRLI_8bit)
					views[var.first].SetShow8bit(nRadarId, nShow);
				if (nPRLIType == RLGM::PRLIType::PRLI_2bit) {
					views[var.first].SetShow2bit(nRadarId, nShow);
					//views[var.first].SetShowTails(nRadarId, nShow);
				}
			}
		}
	}
	bool Interface::GetShowPRLI(int nViewId, int nRadarId, RLGM::PRLIType nPRLIType)
	{
		if (views.count(nViewId) > 0) {
			if (nPRLIType == RLGM::PRLIType::PRLI_Tail)
				return views[nViewId].GetShowTails(nRadarId);
			if (nPRLIType == RLGM::PRLIType::PRLI_8bit)
				return views[nViewId].GetShow8bit(nRadarId);
			if (nPRLIType == RLGM::PRLIType::PRLI_2bit)
				return views[nViewId].GetShow2bit(nRadarId);
		}
		return false;
	}

	//позиционирование Радара: центр
	void Interface::SetRadarPos(int nViewId, int nRadarId, int nX, int nY)
	{
		std::lock_guard<std::mutex> guard{ lock };

		for (auto& var : views)
			if ((var.first == nViewId) || (nViewId == -1))
				views[var.first].SetRadarPosition(nRadarId, nX, nY);
	}
	std::pair<int, int> Interface::GetRadarPos(int nViewId, int nRadarId)
	{
		if (views.count(nViewId))
			if (radars.count(nRadarId))
				return views[nViewId].GetRadarPosition(nRadarId);
		return std::pair<int, int>(0, 0);
	}
	void Interface::AddRadarPos(int nViewId, int nRadarId, int nDX, int nDY)
	{
		std::lock_guard<std::mutex> guard{ lock };

		for (auto& var : views)
			if ((var.first == nViewId) || (nViewId == -1))
				views[var.first].AddRadarPosition(nRadarId, nDX, nDY);
	}
	//позиционирование Радара: масштаб
	void Interface::SetDPP(int nViewId, int nRadarId, float dDPP)
	{
		std::lock_guard<std::mutex> guard{ lock };

		for (auto& var : views)
			if ((var.first == nViewId) || (nViewId == -1))
				views[var.first].SetDPP(nRadarId, dDPP);
	}
	float Interface::GetDPP(int nViewId, int nRadarId)
	{
		if (views.count(nViewId))
			if (radars.count(nRadarId))
				return views[nViewId].GetDPP(nRadarId);
		return 1;
	}
	void Interface::MultiplyDPP(int nViewId, int nRadarId, float value)
	{
		std::lock_guard<std::mutex> guard{ lock };

		for (auto& var : views)
			if ((var.first == nViewId) || (nViewId == -1))
				views[var.first].MultiplyDPP(nRadarId, value);
	}
	void Interface::MultiplyDPPCenter(int nViewId, int nRadarId, float value)
	{
		std::lock_guard<std::mutex> guard{ lock };

		for (auto& var : views)
			if ((var.first == nViewId) || (nViewId == -1))
				views[var.first].MultiplyDPPCenter(nRadarId, value);
	}
	//позиционирование Радара: поворот
	void Interface::SetRotation(int nViewId, int nRadarId, float dRotInGrad)
	{
		std::lock_guard<std::mutex> guard{ lock };
		for (auto& var : views)
			if ((var.first == nViewId) || (nViewId == -1))
				views[var.first].SetRotation(nRadarId, dRotInGrad);
	}
	void Interface::AddRotation(int nViewId, int nRadarId, float delta)
	{
		for (auto& var : views)
			if ((var.first == nViewId) || (nViewId == -1))
				views[var.first].AddRotation(nRadarId, delta);
	}

	//отрисовать окно
	RLGM::ReturnCode Interface::DrawView(int nViewId)
	{
		std::lock_guard<std::mutex> guard{ lock };

		//подсчёт фрэймрэйта
		//frameNumber++;
		//if ((DateTime.Now - frameRateTime).TotalMilliseconds >= 1000)
		//{
		//	frameRateTime = DateTime.Now;
		//	frameRate = frameNumber;
		//	frameNumber = 0;
		//}

		//обновление ПРЛИ Радаров
		//foreach(var item in radars.Keys)
		//	((RLGM_Radar)(radars[item])).UpdatePRLI();

		//отрисовка Окон
		for (auto& var : views)
			if ((var.first == nViewId) || (nViewId == -1))
				views[var.first].Draw();

		return RLGM::ReturnCode::Success;
	}

	void Interface::SetPRLIColorCorrection(int nViewId, int nRadarId, RLGM::PRLIType nPRLIType, int nCount, uint32_t* pPalette)
	{
		std::lock_guard<std::mutex> guard{ lock };

		for (auto& var : views)
			if ((var.first == nViewId) || (nViewId == -1))
			{
				if (nPRLIType == RLGM::PRLIType::PRLI_2bit)
					views[var.first].SetColorCorrection2bit(nRadarId, nCount, pPalette);
				if (nPRLIType == RLGM::PRLIType::PRLI_8bit)
					views[var.first].SetColorCorrection8bit(nRadarId, nCount, pPalette);
				if (nPRLIType == RLGM::PRLIType::PRLI_Tail)
					views[var.first].SetColorCorrectionTails(nRadarId, nCount, pPalette);
			}
	}

	void Interface::ClearTailes(int nRadarId) 
	{
		std::lock_guard<std::mutex> guard{ lock };

		for (auto& var : radars)
			if ((var.first == nRadarId) || (nRadarId == -1))
				radars[var.first].ClearTails();
	}

	//Передать Линейку 8 и 2 битки
	void Interface::Set8bitData(int nRadarId, int start_line, int stop_line, const std::vector<float> &data, int offset)
	{
		std::lock_guard<std::mutex> guard{ lock };

		for (auto& var : radars)
			if ((var.first == nRadarId) || (nRadarId == -1))
				radars[var.first].Set8bitData(start_line, stop_line, data, offset);
	}
	void Interface::Set2bitData(int nRadarId, int start_line, int stop_line, const std::vector<uint8_t> &data, int offset)
	{
		std::lock_guard<std::mutex> guard{ lock };

		for (auto& var : radars)
			if ((var.first == nRadarId) || (nRadarId == -1))
				radars[var.first].Set2bitData(start_line, stop_line, data, offset);
	}

	void Interface::SetUnificationCoefs(int nRadarId, const std::vector<uint16_t>& values) {
		std::lock_guard<std::mutex> guard{ lock };

		for (auto& var : radars)
			if ((var.first == nRadarId) || (nRadarId == -1))
				radars[var.first].SetUnificationCoefs(values);
	}

	//режим текстурирования
	void Interface::SetTextureSamplerType(int nViewId, RLGM::TextureSamplerType value) {
		for (auto& var : views)
			if ((var.first == nViewId) || (nViewId == -1))
				views[var.first].m_8bitTexture_SamplerType = value;
	}
	RLGM::TextureSamplerType Interface::GetTextureSamplerType(int nViewId) {
		if (views.count(nViewId))
			return views[nViewId].m_8bitTexture_SamplerType;
		throw;
	}

	//установить правило склейки 8 биток
	void Interface::Set8bitUnificationRule(int nViewId, RLGM::PRLIUnificationRule value) {
		for (auto& var : views)
			if ((var.first == nViewId) || (nViewId == -1))
				views[var.first].unification8bitRule = value;
	}
	RLGM::PRLIUnificationRule Interface::Get8bitUnificationRule(int nViewId) {
		if (views.count(nViewId))
			return views[nViewId].unification8bitRule;
		throw;
	}


	//возврат Картографического слоя
	ID3D11Texture2D* Interface::GetMapLayer(int nViewId) {
		if (views.count(nViewId))
			return views[nViewId].GetMapLayer();
		return nullptr;
	}
	void Interface::ReleaseMapLayer(int nViewId) {
		if (views.count(nViewId))
			views[nViewId].ReleaseMapLayer();
	}

	//позиционирование нерадарных слоёв
	void Interface::SetSurfacesShift(int nViewId, int nx, int ny) {
		std::lock_guard<std::mutex> guard{ lock };
		for (auto& var : views)
			if ((var.first == nViewId) || (nViewId == -1))
				views[var.first].SetSurfacesShift(nx, ny);
	}
	void Interface::AddSurfacesShift(int nViewId, int dx, int dy) {
		std::lock_guard<std::mutex> guard{ lock };
		for (auto& var : views)
			if ((var.first == nViewId) || (nViewId == -1))
				views[var.first].AddSurfacesShift(dx, dy);
	}
	void Interface::MultiplySurfacesDPP(int nViewId, float value) {
		std::lock_guard<std::mutex> guard{ lock };
		for (auto& var : views)
			if ((var.first == nViewId) || (nViewId == -1))
				views[var.first].MultiplySurfacesDPP(value);
	}
	void Interface::MultiplySurfacesDPPCenter(int nViewId, float value) {
		std::lock_guard<std::mutex> guard{ lock };
		for (auto& var : views)
			if ((var.first == nViewId) || (nViewId == -1))
				views[var.first].MultiplySurfacesDPPCenter(value);
	}

	void Interface::SetShowMap(int nViewId, bool nShow) {
		for (auto& var : views)
			if ((var.first == nViewId) || (nViewId == -1))
				views[var.first].drawMap = nShow;
	}
	void Interface::SetShow8bit(int nViewId, bool nShow) {
		for (auto& var : views)
			if ((var.first == nViewId) || (nViewId == -1))
				views[var.first].draw8bit = nShow;
	}
	void Interface::SetShow2bit(int nViewId, bool nShow) {
		for (auto& var : views)
			if ((var.first == nViewId) || (nViewId == -1))
				views[var.first].draw2bit = nShow;
	}
	void Interface::SetShowTails(int nViewId, bool nShow) {
		for (auto& var : views)
			if ((var.first == nViewId) || (nViewId == -1))
				views[var.first].drawTails = nShow;
	}
	bool Interface::GetShowMap(int nViewId) {
		if (views.count(nViewId))
			return views[nViewId].drawMap;
		throw;
	}
	bool Interface::GetShow8bit(int nViewId) {
		if (views.count(nViewId))
			return views[nViewId].draw8bit;
		throw;
	}
	bool Interface::GetShow2bit(int nViewId) {
		if (views.count(nViewId))
			return views[nViewId].draw2bit;
		throw;
	}
	bool Interface::GetShowTails(int nViewId) {
		if (views.count(nViewId))
			return views[nViewId].drawTails;
		throw;
	}

	void Interface::SetThr2BitForTails(int value) {
		thr2bitForTails = value;
	}

	Interface::Interface()
	{
	}

	Interface::~Interface()
	{
	}
};
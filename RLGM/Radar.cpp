#include "stdafx.h"

#include "Help.h"
#include "Radar.h"
#include "Interface.h"

namespace RLGM 
{
	Radar::Radar() {}
	Radar::Radar(int Id, RLGM::Interface* Owner)
	{
		id = Id;
		owner = Owner;
	}

	void Radar::AddPRLI(RLGM::PRLIType nType, int nPRLISampleCount, int nPRLILineCount)
	{
		if (numLines == -1) //если первая ПРЛИ
		{
			numLines = nPRLILineCount;
			numSamples = nPRLISampleCount;

			//формирование текстур: коэфициенты склейки ПРЛИ
			ID3D11Texture2D* pTexture = nullptr;
			ID3D11ShaderResourceView* pShaderResourceView = nullptr;
			//Create render target texture
			D3D11_TEXTURE2D_DESC textureDesc;
			ZeroMemory(&textureDesc, sizeof(textureDesc));
			textureDesc.Width = numSamples;
			textureDesc.Height = 1;
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 1;
			textureDesc.Format = DXGI_FORMAT_R16_UNORM;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.MiscFlags = 0;
			HRESULT hr = owner->g_pd3dDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);
			if (FAILED(hr)) return;// hr;
			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			// Setup the description of the render target view.
			desc.Format = textureDesc.Format;
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MostDetailedMip = 0;
			desc.Texture2D.MipLevels = textureDesc.MipLevels;
			// Create the render target view.
			hr = owner->g_pd3dDevice->CreateShaderResourceView(pTexture, &desc, &pShaderResourceView);
			if (FAILED(hr)) return;// hr;
			textureUnificationCoefs = RLGM::Texture{ pTexture, nullptr, pShaderResourceView };
			D3D11_BOX region;
			region.left = 0;
			region.right = numSamples;
			region.top = 0;
			region.bottom = 1;
			region.front = 0;
			region.back = 1;
			std::vector<uint16_t> vals(numSamples, UINT16_MAX / 2);
			owner->g_pImmediateContext->UpdateSubresource(textureUnificationCoefs.m_Texture, 0, &region, &vals[0], numSamples, numSamples);

			//формирование геометрии развёртки
			vertexes = new RLGM::PositionedTexturedVertex[3 * numLines];
			for (int line = 0; line < numLines; ++line)
			{
				float x1 = (float)std::cos(RLGM::LineToRadians(line + 0, numLines));
				float y1 = (float)std::sin(RLGM::LineToRadians(line + 0, numLines));
				float x2 = (float)std::cos(RLGM::LineToRadians(line + 1, numLines));
				float y2 = (float)std::sin(RLGM::LineToRadians(line + 1, numLines));

				vertexes[line * 3 + 0] = { DirectX::XMFLOAT3(x1,y1,0),DirectX::XMFLOAT2(1,(line + 0.f) / numLines) };
				vertexes[line * 3 + 1] = { DirectX::XMFLOAT3(x2,y2,0),DirectX::XMFLOAT2(1,(line + 1.f) / numLines) };
				vertexes[line * 3 + 2] = { DirectX::XMFLOAT3(0, 0, 0),DirectX::XMFLOAT2(0,(2 * line + 1.f) / (2.f*numLines)) };
			}
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_IMMUTABLE;
			bd.ByteWidth = sizeof(RLGM::PositionedTexturedVertex) * 3 * numLines;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			D3D11_SUBRESOURCE_DATA InitData;
			ZeroMemory(&InitData, sizeof(InitData));
			InitData.pSysMem = vertexes;
			hr = owner->g_pd3dDevice->CreateBuffer(&bd, &InitData, &vertexBuffer);
		}

		//8 битка
		if (nType == RLGM::PRLIType::PRLI_8bit)
			if (!texture8Bit.m_Texture)
			{
				//Create 8bit texture for radar
				ID3D11Texture2D* pTexture = nullptr;
				ID3D11ShaderResourceView* pShaderResourceView = nullptr;

				D3D11_TEXTURE2D_DESC textureDesc;
				ZeroMemory(&textureDesc, sizeof(textureDesc));
				textureDesc.Width  = numSamples;
				textureDesc.Height = numLines;
				textureDesc.MipLevels = 1;
				textureDesc.ArraySize = 1;
				textureDesc.Format = DXGI_FORMAT_R32_FLOAT;
				textureDesc.SampleDesc.Count = 1;
				textureDesc.SampleDesc.Quality = 0;
				textureDesc.Usage = D3D11_USAGE_DEFAULT;
				textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				textureDesc.CPUAccessFlags = 0;
				textureDesc.MiscFlags = 0;

				HRESULT hr = owner->g_pd3dDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);
				if (FAILED(hr)) return;// hr;

				D3D11_SHADER_RESOURCE_VIEW_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.Format = textureDesc.Format;
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MostDetailedMip = 0;
				desc.Texture2D.MipLevels = textureDesc.MipLevels;
				
				// Create the render target view.
				hr = owner->g_pd3dDevice->CreateShaderResourceView(pTexture, &desc, &pShaderResourceView);
				if (FAILED(hr)) return;// hr;

				texture8Bit = RLGM::Texture { pTexture, nullptr, pShaderResourceView };

				//ZeroPRLI(nType);
			}
		
		//2 битка
		if (nType == RLGM::PRLIType::PRLI_2bit)
			if (!texture2Bit.m_Texture)
			{
				ID3D11Texture2D* pTexture = nullptr;
				ID3D11ShaderResourceView* pShaderResourceView = nullptr;
				ID3D11RenderTargetView* pRenderTargetView;
				D3D11_TEXTURE2D_DESC textureDesc;
				D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
				D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;

				//---> Create 2bit texture for radar				
				ZeroMemory(&textureDesc, sizeof(textureDesc));
				textureDesc.Width = numSamples;
				textureDesc.Height = numLines;
				textureDesc.MipLevels = 1;
				textureDesc.ArraySize = 1;
				textureDesc.Format = DXGI_FORMAT_R8_UNORM;
				textureDesc.SampleDesc.Count = 1;
				textureDesc.SampleDesc.Quality = 0;
				textureDesc.Usage = D3D11_USAGE_DEFAULT;
				textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				textureDesc.CPUAccessFlags = 0;
				textureDesc.MiscFlags = 0;
				HRESULT hr = owner->g_pd3dDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);
				if (FAILED(hr)) return;// hr;
				ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
				shaderResourceViewDesc.Format = textureDesc.Format;
				shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
				shaderResourceViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;
				hr = owner->g_pd3dDevice->CreateShaderResourceView(pTexture, &shaderResourceViewDesc, &pShaderResourceView);
				if (FAILED(hr)) return;// hr;
				texture2Bit = RLGM::Texture{ pTexture, nullptr, pShaderResourceView };
				//<--- Create 2bit texture for radar

				//---> Create Tails texture for radar
				for (int i = 0; i < 2; i++)
				{
					ZeroMemory(&textureDesc, sizeof(textureDesc));
					textureDesc.Width = numSamples;
					textureDesc.Height = numLines;
					textureDesc.MipLevels = 1;
					textureDesc.ArraySize = 1;
					textureDesc.Format = DXGI_FORMAT_R8G8_UNORM;
					textureDesc.SampleDesc.Count = 1;
					textureDesc.SampleDesc.Quality = 0;
					textureDesc.Usage = D3D11_USAGE_DEFAULT;
					textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
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
					ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
					renderTargetViewDesc.Format = textureDesc.Format;
					renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
					renderTargetViewDesc.Texture2D.MipSlice = 0;
					hr = owner->g_pd3dDevice->CreateRenderTargetView(pTexture, &renderTargetViewDesc, &pRenderTargetView);
					if (FAILED(hr)) return;// hr;
					textureTail[i] = RLGM::Texture{ pTexture, pRenderTargetView, pShaderResourceView };
				}
				ClearTails();
			}
	}

	void Radar::ClearTails() {
		if (textureTail[0].m_Texture != nullptr) //если существуют Хвосты
		{
			float black[4]{ 0,0,0,0 };
			owner->g_pImmediateContext->OMSetRenderTargets(1, &textureTail[0].m_renderTargetView, nullptr);			
			owner->g_pImmediateContext->ClearRenderTargetView(textureTail[0].m_renderTargetView, black);
			owner->g_pImmediateContext->OMSetRenderTargets(1, &textureTail[1].m_renderTargetView, nullptr);
			owner->g_pImmediateContext->ClearRenderTargetView(textureTail[1].m_renderTargetView, black);
		}
	}

	//Обновление сектора [start_line, end_line] 8 битки
	void Radar::Set8bitData(int start_line, int stop_line, const std::vector<float> &data, int offset)
	{
		D3D11_BOX region;
		region.left = 0;
		region.right = numSamples;
		region.top = start_line;
		region.bottom = stop_line + 1;
		region.front = 0;
		region.back = 1;
		owner->g_pImmediateContext->UpdateSubresource(texture8Bit.m_Texture, 0, &region, &data[offset], numSamples * sizeof(float), 0);
	}
	//Обновление сектора [start_line, end_line] 8 битки
	void Radar::Set2bitData(int start_line, int stop_line, const std::vector<uint8_t> &data, int offset)
	{
		D3D11_BOX region;
		region.left = 0;
		region.right = numSamples;
		region.top = start_line;
		region.bottom = stop_line + 1;
		region.front = 0;
		region.back = 1;
		owner->g_pImmediateContext->UpdateSubresource(texture2Bit.m_Texture, 0, &region, &data[offset], numSamples, numSamples * numLines);
	}

	//Отрисовка 8 битки
	void Radar::Draw8bit(RLGM::RadarPositioning geom, float aspect, ID3D11PixelShader* ps, ID3D11VertexShader* vs, ID3D11SamplerState* sampler/*, RadarDrawMode drawMode*/)
	{
		if (texture8Bit.m_Texture) //если существует 8 битка
		{
			// Set vertex buffer
			UINT stride = sizeof(RLGM::PositionedTexturedVertex);
			UINT offset = 0;
			owner->g_pImmediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
			owner->g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			
			// Set shaders
			owner->g_pImmediateContext->VSSetShader(vs, nullptr, 0);
			RLGM::ConstantBuffer1 cb1;			//вычисление матрицы позиционирования
			cb1.mViewWorldProjection = DirectX::XMMatrixRotationZ(-geom.rotation * ((float)RLGM::M_PI / 180)) *
									   DirectX::XMMatrixScaling(aspect * geom.radius, geom.radius, 1) *
									   DirectX::XMMatrixTranslation(geom.centerX, geom.centerY, 0);
			owner->g_pImmediateContext->UpdateSubresource(owner->constantBuffer1, 0, nullptr, &cb1, 0, 0);
			owner->g_pImmediateContext->VSSetConstantBuffers(0, 1, &owner->constantBuffer1);
			owner->g_pImmediateContext->PSSetShader(ps, nullptr, 0);
			RLGM::ConstantBuffer3 cb3{ id };
			owner->g_pImmediateContext->UpdateSubresource(owner->constantBuffer3, 0, nullptr, &cb3, 0, 0);
			owner->g_pImmediateContext->PSSetConstantBuffers(0, 1, &owner->constantBuffer3);
			owner->g_pImmediateContext->PSSetShaderResources(0, 1, &texture8Bit.m_shaderResourceView);
			owner->g_pImmediateContext->PSSetShaderResources(1, 1, &textureUnificationCoefs.m_shaderResourceView);
			owner->g_pImmediateContext->PSSetSamplers(0, 1, &sampler);
			owner->g_pImmediateContext->PSSetSamplers(1, 1, &owner->samplerPoint);
			owner->g_pImmediateContext->Draw(3 * numLines, 0);
		}
	}
	void Radar::Draw2bit(RLGM::RadarPositioning geom, float aspect, ID3D11PixelShader* ps, ID3D11VertexShader* vs, ID3D11SamplerState* sampler/*, RadarDrawMode drawMode*/)
	{
		if (texture2Bit.m_Texture) //если существует 2 битка
		{
			// Set vertex buffer
			UINT stride = sizeof(RLGM::PositionedTexturedVertex);
			UINT offset = 0;
			owner->g_pImmediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
			owner->g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			// Set shaders
			owner->g_pImmediateContext->VSSetShader(vs, nullptr, 0);
			RLGM::ConstantBuffer1 cb1;			//вычисление матрицы позиционирования
			cb1.mViewWorldProjection = DirectX::XMMatrixRotationZ(-geom.rotation * ((float)RLGM::M_PI / 180)) *
				DirectX::XMMatrixScaling(aspect * geom.radius, geom.radius, 1) *
				DirectX::XMMatrixTranslation(geom.centerX, geom.centerY, 0);
			owner->g_pImmediateContext->UpdateSubresource(owner->constantBuffer1, 0, nullptr, &cb1, 0, 0);
			owner->g_pImmediateContext->VSSetConstantBuffers(0, 1, &owner->constantBuffer1);
			owner->g_pImmediateContext->PSSetShader(ps, nullptr, 0);
			RLGM::ConstantBuffer3 cb3{ id };
			owner->g_pImmediateContext->UpdateSubresource(owner->constantBuffer3, 0, nullptr, &cb3, 0, 0);
			owner->g_pImmediateContext->PSSetConstantBuffers(0, 1, &owner->constantBuffer3);
			owner->g_pImmediateContext->PSSetShaderResources(0, 1, &texture2Bit.m_shaderResourceView);
			owner->g_pImmediateContext->PSSetShaderResources(1, 1, &textureUnificationCoefs.m_shaderResourceView);
			owner->g_pImmediateContext->PSSetSamplers(0, 1, &sampler);
			owner->g_pImmediateContext->PSSetSamplers(1, 1, &owner->samplerPoint);
			owner->g_pImmediateContext->Draw(3 * numLines, 0);
		}
	}
	void Radar::DrawTail(RLGM::RadarPositioning geom, float aspect, ID3D11PixelShader* ps, ID3D11VertexShader* vs, ID3D11SamplerState* sampler/*, RadarDrawMode drawMode*/)
	{
		if (textureTail[1 - textureTailIndexForCalc].m_Texture) //если существует 2 битка
		{
			// Set vertex buffer
			UINT stride = sizeof(RLGM::PositionedTexturedVertex);
			UINT offset = 0;
			owner->g_pImmediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
			owner->g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			// Set shaders
			owner->g_pImmediateContext->VSSetShader(vs, nullptr, 0);
			RLGM::ConstantBuffer1 cb1;			//вычисление матрицы позиционирования
			cb1.mViewWorldProjection = DirectX::XMMatrixRotationZ(-geom.rotation * ((float)RLGM::M_PI / 180)) *
				DirectX::XMMatrixScaling(aspect * geom.radius, geom.radius, 1) *
				DirectX::XMMatrixTranslation(geom.centerX, geom.centerY, 0);
			owner->g_pImmediateContext->UpdateSubresource(owner->constantBuffer1, 0, nullptr, &cb1, 0, 0);
			owner->g_pImmediateContext->VSSetConstantBuffers(0, 1, &owner->constantBuffer1);
			owner->g_pImmediateContext->PSSetShader(ps, nullptr, 0);
			RLGM::ConstantBuffer3 cb3{ id };
			owner->g_pImmediateContext->UpdateSubresource(owner->constantBuffer3, 0, nullptr, &cb3, 0, 0);
			owner->g_pImmediateContext->PSSetConstantBuffers(0, 1, &owner->constantBuffer3);
			owner->g_pImmediateContext->PSSetShaderResources(0, 1, &textureTail[1 - textureTailIndexForCalc].m_shaderResourceView);
			owner->g_pImmediateContext->PSSetSamplers(0, 1, &sampler);
			owner->g_pImmediateContext->Draw(3 * numLines, 0);
		}
	}
	void Radar::Draw2bitTail(RLGM::RadarPositioning geom, float aspect, ID3D11PixelShader* ps, ID3D11VertexShader* vs, ID3D11SamplerState* sampler/*, RadarDrawMode drawMode*/)
	{
		if (textureTail[1 - textureTailIndexForCalc].m_Texture) //если существует 2 битка хвоста
		{
			// Set vertex buffer
			UINT stride = sizeof(RLGM::PositionedTexturedVertex);
			UINT offset = 0;
			owner->g_pImmediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
			owner->g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			// Set shaders
			owner->g_pImmediateContext->VSSetShader(vs, nullptr, 0);
			RLGM::ConstantBuffer1 cb1;			//вычисление матрицы позиционирования
			cb1.mViewWorldProjection = DirectX::XMMatrixRotationZ(-geom.rotation * ((float)RLGM::M_PI / 180)) *
				DirectX::XMMatrixScaling(aspect * geom.radius, geom.radius, 1) *
				DirectX::XMMatrixTranslation(geom.centerX, geom.centerY, 0);
			owner->g_pImmediateContext->UpdateSubresource(owner->constantBuffer1, 0, nullptr, &cb1, 0, 0);
			owner->g_pImmediateContext->VSSetConstantBuffers(0, 1, &owner->constantBuffer1);
			owner->g_pImmediateContext->PSSetShader(ps, nullptr, 0);
			RLGM::ConstantBuffer3 cb3{ id };
			owner->g_pImmediateContext->UpdateSubresource(owner->constantBuffer3, 0, nullptr, &cb3, 0, 0);
			owner->g_pImmediateContext->PSSetConstantBuffers(0, 1, &owner->constantBuffer3);
			owner->g_pImmediateContext->PSSetShaderResources(0, 1, &textureTail[1 - textureTailIndexForCalc].m_shaderResourceView);
			owner->g_pImmediateContext->PSSetShaderResources(1, 1, &textureUnificationCoefs.m_shaderResourceView);
			owner->g_pImmediateContext->PSSetSamplers(0, 1, &sampler);
			owner->g_pImmediateContext->PSSetSamplers(1, 1, &owner->samplerPoint);
			owner->g_pImmediateContext->Draw(3 * numLines, 0);
		}
	}

	void Radar::SetUnificationCoefs(const std::vector<uint16_t>& values) {
		D3D11_BOX region;
		region.left = 0;
		region.right = numSamples;
		region.top = 0;
		region.bottom = 1;
		region.front = 0;
		region.back = 1;
		owner->g_pImmediateContext->UpdateSubresource(textureUnificationCoefs.m_Texture, 0, &region, &values[0], numSamples, numSamples);
	}

	void Radar::CalcTails(ID3D11PixelShader* ps, ID3D11VertexShader* vs, int thr2bitForTails)
	{
		if (textureTail[0].m_Texture != nullptr)
		{
			// Disable alpha blending
			UINT sampleMask = 0xffffffff;
			owner->g_pImmediateContext->OMSetBlendState(owner->g_pBlendStateNoBlend, nullptr, sampleMask);
			// Set the viewport
			D3D11_VIEWPORT vp{ 0, 0, numSamples, numLines, 0, 1 };
			owner->g_pImmediateContext->RSSetViewports(1, &vp);

			owner->g_pImmediateContext->OMSetRenderTargets(1, &textureTail[textureTailIndexForCalc].m_renderTargetView, nullptr);

			// Set vertex buffer
			UINT stride = sizeof(RLGM::PositionedTexturedVertex);
			UINT offset = 0;
			owner->g_pImmediateContext->IASetVertexBuffers(0, 1, &owner->vbQuadScreen, &stride, &offset);
			owner->g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			// Set shaders
			owner->g_pImmediateContext->VSSetShader(vs, nullptr, 0);
			RLGM::ConstantBuffer1 cb1;			//вычисление матрицы позиционирования
			cb1.mViewWorldProjection = DirectX::XMMatrixIdentity();
			owner->g_pImmediateContext->UpdateSubresource(owner->constantBuffer1, 0, nullptr, &cb1, 0, 0);
			owner->g_pImmediateContext->VSSetConstantBuffers(0, 1, &owner->constantBuffer1);
			owner->g_pImmediateContext->PSSetShader(ps, nullptr, 0);
			RLGM::ConstantBuffer3 cb3{ thr2bitForTails };
			owner->g_pImmediateContext->UpdateSubresource(owner->constantBuffer3, 0, nullptr, &cb3, 0, 0);
			owner->g_pImmediateContext->PSSetConstantBuffers(0, 1, &owner->constantBuffer3);
			owner->g_pImmediateContext->PSSetShaderResources(0, 1, &texture2Bit.m_shaderResourceView);
			owner->g_pImmediateContext->PSSetShaderResources(1, 1, &textureTail[1 - textureTailIndexForCalc].m_shaderResourceView);
			owner->g_pImmediateContext->PSSetSamplers(0, 1, &owner->samplerPoint);
			owner->g_pImmediateContext->PSSetSamplers(1, 1, &owner->samplerPoint);
			owner->g_pImmediateContext->Draw(6, 0);

			textureTailIndexForCalc = 1 - textureTailIndexForCalc;
		}
	}

	Radar::~Radar()
	{
	}
};

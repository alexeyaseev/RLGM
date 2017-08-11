#pragma once

#include <mutex>
#include <set>
#include <map>
#include <vector>
#include <exception>
#include <iostream>
#include <d3dcompiler.h>
#include <d3d11_1.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <cmath>
#include <fstream>

namespace RLGM
{
	const double M_PI = std::acos(-1);

	struct PositionedTexturedVertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 textureCoordinates;
	};

	//���������� � ���������������� ������
	struct RadarPositioning
	{
		float centerX; //����� �����: ���������� X
		float centerY; //����� �����: ���������� Y
		float radius; //������ �����
		float rotation; //������� �����
	};

	//��� ����������� ����
	enum class PRLIUnificationRule { MAX = 0, LINEAR = 1 };
	//��� ����������� �������
	enum class TextureSamplerType { POINT, LINEAR };
	//��� ����
	enum class PRLIType { PRLI_2bit, PRLI_8bit, PRLI_Tail };
	//���� �������� ������������ �������
	enum class ReturnCode
	{
		Success,
		Fail,
		D3DDeviceCreationError,
		ShaderLoadError,
		D3DDeviceTuneError,
		CreateSpriteError,
		CreateTailsCalcTimer,
		CreateViewError,
		CreateRadarError,
		DrawViewError,
		AddPRLIError
	};

	//������� ������ ������� � �������
	double LineToRadians(int line, int lineCount);

	struct Texture {
		ID3D11Texture2D*		  m_Texture = nullptr;
		ID3D11RenderTargetView*	  m_renderTargetView    = nullptr;
		ID3D11ShaderResourceView* m_shaderResourceView  = nullptr;
	};
};

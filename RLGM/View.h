#pragma once

#include "Help.h"

namespace RLGM {
	class Interface;
}

namespace RLGM
{
	class View
	{
	public:
		RLGM::Interface* owner;
		
		IDXGISwapChain*          g_pSwapChain = nullptr;
		IDXGISwapChain1*         g_pSwapChain1 = nullptr;		
		ID3D11RenderTargetView*  g_pRenderTargetView = nullptr;
		ID3D11Texture2D*		 g_pBackBuffer = nullptr;
		

		std::map<int, RLGM::RadarPositioning> geoms; // ���������� � ���������������� �������
		RLGM::RadarPositioning mapGeom{ 0,0,1,0 }; // ���������� � ���������������� �����
		RLGM::Texture colorCorrection8bitTexture; // �������������� 8 �����
		RLGM::Texture colorCorrection2bitTexture; // �������������� 2 �����
		RLGM::Texture colorCorrectionTailTexture; // �������������� �������
		RLGM::Texture layerMap[2]; // ���� �����
		int layerMapIndexToDraw = 0;

		//������ ���������������
		RLGM::TextureSamplerType m_8bitTexture_SamplerType = RLGM::TextureSamplerType::POINT;
		//������� ������� 8 �����
		RLGM::PRLIUnificationRule unification8bitRule = RLGM::PRLIUnificationRule::MAX;
		//������� ������� 2 �����
		RLGM::PRLIUnificationRule unification2bitRule = RLGM::PRLIUnificationRule::MAX;

		bool draw8bit = true;   // ���� ��������� ���� 8 �����
		std::map<int, bool> show8bit; // ���� ��������� 8 ����� �� ������
		bool draw2bit = false;   // ���� ��������� ���� 2 �����
		std::map<int, bool> show2bit; // ���� ��������� 2 ����� �� ������
		bool drawMap = false;   // ���� ��������� �����
		std::map<int, bool> showTails; // ���� ��������� ������ �� ������
		bool drawTails = false;   // ���� ��������� ������

		HWND hWnd; // ����� ����
		int width; // ������ ����
		int height; // ������ ����
		double aspectRatio; //����������� ������		
		double scale = 1;
		RLGM::GeoCoord geoPosition; //�������������� ������������
		float extensionCoef = 1.0f;

		//���� ������ ��� ���������
		std::vector<RLGM::Texture> textureRadarCurrent;
		RLGM::Texture textureFinalWithoutColorCorrection;
		RLGM::Texture textureFinalWithoutColorCorrectionHelp1;
		RLGM::Texture textureFinalWithoutColorCorrectionHelp2;
		RLGM::Texture textureFinal8bit;
		RLGM::Texture textureFinal2bit;
		RLGM::Texture textureFinalTail;

		View();
		View(HWND HWnd, RLGM::Interface* owner, double height, double longitude, double latitude);
		void AddRadar(int nRadarId);
		void RemoveRadar(int nRadarId);
		
		void AddPixelOffset(int nX, int nY);
		RLGM::GeoCoord xyToGeo(int x, int y);

		//���������������� ������: �����, �������, �������
		void SetRadarPosition(int nRadarId, int nX, int nY);
		std::pair<int, int> GetRadarPosition(int nRadarId);
		void AddRadarPosition(int nRadarId, int nDX, int nDY);
		void SetDPP(int nRadarId, float dDPP);
		float GetDPP(int nRadarId);
		void MultiplyDPP(int nRadarId, float value);
		void MultiplyDPPCenter(int nRadarId, float value);
		void SetRotation(int nRadarId, float rot);
		void AddRotation(int nRadarId, float rot);
		
		void SetShow8bit(int nRadarId, bool value);
		bool GetShow8bit(int nRadarId);
		void SetShow2bit(int nRadarId, bool value);
		bool GetShow2bit(int nRadarId);
		void SetShowTails(int nRadarId, bool value);
		bool GetShowTails(int nRadarId);

		void SetColorCorrection8bit(int nRadarId, int nCount, uint32_t* value);
		void SetColorCorrection2bit(int nRadarId, int nCount, uint32_t* value);
		void SetColorCorrectionTails(int nRadarId, int nCount, uint32_t* value);

		//���������� ����
		RLGM::ReturnCode Draw();
		void InitializeGraphics();
		
		~View();

		//������� ����������������� ����
		ID3D11Texture2D* GetMapLayer();
		void ReleaseMapLayer();		

		//���������������� ���������� ����
		void SetSurfacesShift(int nx, int ny);
		void AddSurfacesShift(int dx, int dy);
		void MultiplySurfacesDPP(float value);
		void MultiplySurfacesDPPCenter(float value);
	};
};


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
		

		std::map<int, RLGM::RadarPositioning> geoms; // информация о позиционировании Радаров
		RLGM::RadarPositioning mapGeom{ 0,0,1,0 }; // информация о позиционировании карты
		RLGM::Texture colorCorrection8bitTexture; // цветокоррекция 8 биток
		RLGM::Texture colorCorrection2bitTexture; // цветокоррекция 2 биток
		RLGM::Texture colorCorrectionTailTexture; // цветокоррекция хвостов
		RLGM::Texture layerMap[2]; // слой карты
		int layerMapIndexToDraw = 0;

		//фильтр текстурирования
		RLGM::TextureSamplerType m_8bitTexture_SamplerType = RLGM::TextureSamplerType::POINT;
		//правило склейки 8 биток
		RLGM::PRLIUnificationRule unification8bitRule = RLGM::PRLIUnificationRule::MAX;
		//правило склейки 2 биток
		RLGM::PRLIUnificationRule unification2bitRule = RLGM::PRLIUnificationRule::MAX;

		bool draw8bit = true;   // флаг рисования слоя 8 битки
		std::map<int, bool> show8bit; // флаг рисования 8 битки по радару
		bool draw2bit = false;   // флаг рисования слоя 2 битки
		std::map<int, bool> show2bit; // флаг рисования 2 битки по радару
		bool drawMap = false;   // флаг рисования карты
		std::map<int, bool> showTails; // флаг рисования хвоста по радару
		bool drawTails = false;   // флаг рисования хвоста

		HWND hWnd; // хендл окна
		int width; // ширина окна
		int height; // высота окна
		double aspectRatio; //соотношение сторон		
		double scale = 1;
		RLGM::GeoCoord geoPosition; //географическое расположение
		float extensionCoef = 1.0f;

		//слои Радара при отрисовке
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

		//позиционирование Радара: центр, масштаб, поворот
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

		//отрисовать Окно
		RLGM::ReturnCode Draw();
		void InitializeGraphics();
		
		~View();

		//возврат Картографического слоя
		ID3D11Texture2D* GetMapLayer();
		void ReleaseMapLayer();		

		//позиционирование нерадарных слоёв
		void SetSurfacesShift(int nx, int ny);
		void AddSurfacesShift(int dx, int dy);
		void MultiplySurfacesDPP(float value);
		void MultiplySurfacesDPPCenter(float value);
	};
};


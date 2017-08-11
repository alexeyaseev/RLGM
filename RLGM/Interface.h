#pragma once

#include "Help.h"
#include "Radar.h"
#include "View.h"

namespace RLGM
{
	struct ConstantBuffer1 //for vertex shader
	{
		DirectX::XMMATRIX mViewWorldProjection;
	};
	__declspec(align(16))
	struct ConstantBuffer3 //for pixel shader
	{		
		int radarId;
	};
	__declspec(align(16))
	struct ConstantBuffer4 //for pixel shader
	{		
		int numRadars;			 //number of radars
		int unification8bitRule; //unification rule: 0 - max, 1 - linear combination
	};

	class __declspec(dllexport) Interface
	{
	public:
		std::mutex lock;

		bool m_bExit = false; //exit flag
		
		//tails
		std::thread tailsCalcThread;
		int tailsCalcPeriod_ms;
		int thr2bitForTails = 1;

		std::map<int, RLGM::View> views;
		std::map<int, RLGM::Radar> radars;

		//Direct3d related
		D3D_FEATURE_LEVEL       g_featureLevel;
		ID3D11Device*           g_pd3dDevice = nullptr;
		ID3D11Device1*          g_pd3dDevice1 = nullptr;
		ID3D11DeviceContext*    g_pImmediateContext = nullptr;
		ID3D11DeviceContext1*   g_pImmediateContext1 = nullptr;
		ID3D11InputLayout*		g_pVertexLayout = nullptr;
		ID3D11RasterizerState*  g_rasterizerState = nullptr;
		std::map<std::wstring, ID3D11VertexShader*> vsShaders;
		std::map<std::wstring, ID3D11PixelShader*> psShaders;
		ID3D11BlendState* g_pBlendStateBlend = nullptr;
		ID3D11BlendState* g_pBlendStateNoBlend = nullptr;
		ID3D11SamplerState* samplerLinear = nullptr;
		ID3D11SamplerState* samplerPoint = nullptr;
		ID3D11Buffer* constantBuffer1 = nullptr;
		ID3D11Buffer* constantBuffer3 = nullptr;
		ID3D11Buffer* constantBuffer4 = nullptr;
		ID3D11Buffer* vbQuadScreen = nullptr;


		//Картинки
		/*public Hashtable pictures = new Hashtable();

		//Спрайт для рисования Картинок
		public Sprite sprite;
		//параметры представления
		public PresentParameters presentationParams = new PresentParameters();

		//Таймер для вычисления Хвостов
		Timer timerTail;
		//флаг указывающий что идёт вычисление хвостов в данный момент
		volatile bool calcTailsWorking = false;
		//объект для синхронизации: вычисление Хвостов
		object obSyncCalcTails = new object();
		//флаг указывающий что хвосты надо вычислять
		bool calcTailesMode = true;

		//объект для синхронизации: вычисление Хвостов, очистка Хвостов, отрисовка Окон
		object obLock = new object();

		//подсчёт фреймрэйта
		int frameNumber;
		public float frameRate;
		DateTime frameRateTime;*/

		RLGM::ReturnCode CreateDevice();
		RLGM::ReturnCode LoadShaders();
		HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

		Interface();
		~Interface();

		//стартовая инициализация/деинициализация
		RLGM::ReturnCode Init(int tails_calc_period_ms);
		void DeInit();
		//создать, удалить Окно
		RLGM::ReturnCode CreateView(HWND hWnd, int nViewId);
		void DeleteView(int nViewId);
		//создать, удалить Радар (should be >0)
		RLGM::ReturnCode CreateRadar(int nRadarId); //nRadarId should be >0
		void DeleteRadar(int nRadarId);
		//добавить ПРЛИ в Радар
		RLGM::ReturnCode AddPRLI(int nRadarId, RLGM::PRLIType nType, int nPRLISampleCount, int nPRLILineCount);

		//вычисление Хвостов
		void CalcTails(int period_ms);

		//позиционирование Радара: центр
		void SetRadarPos(int nViewId, int nRadarId, int nX, int nY);
		std::pair<int, int> GetRadarPos(int nViewId, int nRadarId);
		void AddRadarPos(int nViewId, int nRadarId, int nDX, int nDY);
		//позиционирование Радара: масштаб
		void SetDPP(int nViewId, int nRadarId, float dDPP);
		float GetDPP(int nViewId, int nRadarId);
		void MultiplyDPP(int nViewId, int nRadarId, float value);
		void MultiplyDPPCenter(int nViewId, int nRadarId, float value);
		//позиционирование Радара: поворот
		void SetRotation(int nViewId, int nRadarId, float dRotInGrad);
		void AddRotation(int nViewId, int nRadarId, float delta);

		//void RestoreDevice();

		//отрисовать окно
		RLGM::ReturnCode DrawView(int nViewId);

		//настроить Цветокоррекцию
		void SetPRLIColorCorrection(int nViewId, int nRadarId, RLGM::PRLIType nPRLIType, int nCount, uint32_t* pPalette);

		//настроить коэф-ты склейки
		void SetUnificationCoefs(int nRadarId, const std::vector<uint16_t>& values);

		//настроить отображение ПРЛИ
		void SetShowPRLI(int nViewId, int nRadarId, RLGM::PRLIType nPRLIType, bool nShow);
		bool GetShowPRLI(int nViewId, int nRadarId, RLGM::PRLIType nPRLIType);

		//Передать Линейку 8 и 2 битки
		void Set8bitData(int nRadarId, int start_line, int stop_line, const std::vector<float> &data, int offset);
		void Set2bitData(int nRadarId, int start_line, int stop_line, const std::vector<uint8_t> &data, int offset);
	
		//возврат Картографического слоя
		ID3D11Texture2D* GetMapLayer(int nViewId);
		void ReleaseMapLayer(int nViewId);

		//настроить режим текстурирования
		void SetTextureSamplerType(int nViewId, RLGM::TextureSamplerType value);
		RLGM::TextureSamplerType GetTextureSamplerType(int nViewId);

		//настроить отображение слоёв
		void SetShowMap(int nViewId, bool nShow);
		void SetShow8bit(int nViewId, bool nShow);
		void SetShow2bit(int nViewId, bool nShow);
		void SetShowTails(int nViewId, bool nShow);
		bool GetShowMap(int nViewId);
		bool GetShow8bit(int nViewId);
		bool GetShow2bit(int nViewId);
		bool GetShowTails(int nViewId);
		
		//очистить Хвосты
		void ClearTailes(int nRadarId);

		//установить правило склейки 8 биток
		void Set8bitUnificationRule(int nViewId, RLGM::PRLIUnificationRule value);
		RLGM::PRLIUnificationRule Get8bitUnificationRule(int nViewId);

		//позиционирование нерадарных слоёв
		void SetSurfacesShift(int nViewId, int nx, int ny);
		void AddSurfacesShift(int nViewId, int dx, int dy);
		void MultiplySurfacesDPP(int nViewId, float value);
		void MultiplySurfacesDPPCenter(int nViewId, float value);

		//порог 2битки для вычисления хвоста
		void SetThr2BitForTails(int value);
	};
};


//возврат Верхнего слоя 2
//public IntPtr GetTopSurface2(int nViewId);
//public void ReleaseTopSurface2(int nViewId)

//возврат Верхнего слоя 1
//public IntPtr GetTopSurface1(int nViewId)
//public IntPtr GetTopSurface1HDC(int nViewId)
//public void ReleaseTopSurface1(int nViewId)
//public void ReleaseTopSurface1HDC(int nViewId, IntPtr hDC);
//очистить Верхний слой 1
//public void ClearTop1(int nViewId)
//очистить Верхний слой 2
//public void ClearTop2(int nViewId)

//установить фоновый цвет
//public void SetBackColor(int nViewId, int colorARGBA)

//настроить отображение верхнего слоя 1
//public void SetShowTop1(int nViewId, int nShow)
//public bool GetShowTop1(int nViewId, int nShow)

//настроить отображение верхнего слоя 2
//public void SetShowTop2(int nViewId, int nShow)
//public bool GetShowTop2(int nViewId, int nShow)

//настроить отображение Картинок
//public void SetShowPictures(int nViewId, int nShow)
//public bool GetShowPictures(int nViewId)

//таймаут для очистки ПРЛИ при отсутствии данных
//public void SetTimeoutForClearing(int nRadarId, int nMseconds)

//установить правило склейки 2 биток
//public void Set2bitIntegrationRule(int nViewId, int nRule)
//public void Set2bitIntegrationRule(int nViewId, IntegrationRuleType rule)

//настроить режим отображения радара
//public void SetRadarDrawMode(int nViewId, int nMode)

//загрузить Картинку
//public void SetPicture(int nPictureId, string bstrFileName, out uint width, out uint height)
//позиционирование Картинки: центр
//public void SetPicturePos(int nViewId, int nX, int nY, int nPictureId)
//удалить Картинку
//public void DeletePicture(int nViewId, int nPictureId)


//Рисование Значков
//if (drawPictures)
//{
//	owner.sprite.Begin(SpriteFlags.AlphaBlend);
//	foreach(var item in((Hashtable)(picturesPosition.Clone())).Keys)
//		owner.sprite.Draw((Texture)owner.pictures[item], new Vector3(((Texture)(owner.pictures[item])).GetLevelDescription(0).Width / 2, ((Texture)(owner.pictures[item])).GetLevelDescription(0).Height / 2, 0), new Vector3(((Point)(picturesPosition[item])).X + picturesPositionOffset.X, ((Point)(picturesPosition[item])).Y + picturesPositionOffset.Y, 0), new Color4(1, 1, 1, 1));
//	owner.sprite.End();
//}
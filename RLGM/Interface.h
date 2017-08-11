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
		
		ID3D11Buffer* vbQuadScreen = nullptr;

		int thr2bitForTails = 1;

		//Окна
		std::map<int, RLGM::View> views;
		//Радары
		std::map<int, RLGM::Radar> radars;

		//Устройство рисования Direct3d
		D3D_FEATURE_LEVEL       g_featureLevel;
		ID3D11Device*           g_pd3dDevice = nullptr;
		ID3D11Device1*          g_pd3dDevice1 = nullptr;
		ID3D11DeviceContext*    g_pImmediateContext = nullptr;
		ID3D11DeviceContext1*   g_pImmediateContext1 = nullptr;
		ID3D11InputLayout*		g_pVertexLayout = nullptr;
		ID3D11RasterizerState*  g_rasterizerState = nullptr;
		std::map<std::wstring, ID3D11VertexShader*> vsShaders; //Шейдеры
		std::map<std::wstring, ID3D11PixelShader*> psShaders;  //Шейдеры
		ID3D11BlendState* g_pBlendStateBlend = nullptr;
		ID3D11BlendState* g_pBlendStateNoBlend = nullptr;

		ID3D11SamplerState* samplerLinear = nullptr;
		ID3D11SamplerState* samplerPoint = nullptr;

		ID3D11Buffer* constantBuffer1 = nullptr;
		ID3D11Buffer* constantBuffer3 = nullptr;
		ID3D11Buffer* constantBuffer4 = nullptr;

		std::thread tailsCalcThread;
		int tailsCalcPeriod_ms;

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

		void CalcTails(int period_ms);

	public:
		Interface();
		~Interface();

		//стартовая инициализация/деинициализация
		RLGM::ReturnCode Init(int tails_calc_period_ms);
		void DeInit();
		//создать, удалить Окно
		RLGM::ReturnCode CreateView(HWND hWnd, int nViewId);
		void DeleteView(int nViewId);
		//создать, удалить Радар (should be >0)
		RLGM::ReturnCode CreateRadar(int nRadarId);
		void DeleteRadar(int nRadarId);
		//добавить ПРЛИ в Радар
		RLGM::ReturnCode AddPRLI(int nRadarId, RLGM::PRLIType nType, int nPRLISampleCount, int nPRLILineCount);

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

		void SetShowPRLI(int nViewId, int nRadarId, RLGM::PRLIType nPRLIType, bool nShow);
		bool GetShowPRLI(int nViewId, int nRadarId, RLGM::PRLIType nPRLIType);

		void SetPRLIColorCorrection(int nViewId, int nRadarId, RLGM::PRLIType nPRLIType, int nCount, uint32_t* pPalette);

		void SetUnificationCoefs(int nRadarId, const std::vector<uint16_t>& values);

		//отрисовать окно
		RLGM::ReturnCode DrawView(int nViewId);

		void ClearTailes(int nRadarId);

		//Передать Линейку 8 и 2 битки
		void Set8bitData(int nRadarId, int start_line, int stop_line, const std::vector<float> &data, int offset);
		void Set2bitData(int nRadarId, int start_line, int stop_line, const std::vector<uint8_t> &data, int offset);

		//режим текстурирования
		void SetTextureSamplerType(int nViewId, RLGM::TextureSamplerType value);
		RLGM::TextureSamplerType GetTextureSamplerType(int nViewId);

		//установить правило склейки 8 биток
		void Set8bitUnificationRule(int nViewId, RLGM::PRLIUnificationRule value);
		RLGM::PRLIUnificationRule Get8bitUnificationRule(int nViewId);

		//возврат Картографического слоя
		ID3D11Texture2D* GetMapLayer(int nViewId);
		void ReleaseMapLayer(int nViewId);
		
		//позиционирование нерадарных слоёв
		void SetSurfacesShift(int nViewId, int nx, int ny);
		void AddSurfacesShift(int nViewId, int dx, int dy);
		void MultiplySurfacesDPP(int nViewId, float value);
		void MultiplySurfacesDPPCenter(int nViewId, float value);

		void SetShowMap(int nViewId, bool nShow);
		void SetShow8bit(int nViewId, bool nShow);
		void SetShow2bit(int nViewId, bool nShow);
		void SetShowTails(int nViewId, bool nShow);
		bool GetShowMap(int nViewId);
		bool GetShow8bit(int nViewId);
		bool GetShow2bit(int nViewId);
		bool GetShowTails(int nViewId);

		void SetThr2BitForTails(int value);
	};
};


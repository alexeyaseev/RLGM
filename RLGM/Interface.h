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


		//��������
		/*public Hashtable pictures = new Hashtable();

		//������ ��� ��������� ��������
		public Sprite sprite;
		//��������� �������������
		public PresentParameters presentationParams = new PresentParameters();

		//������ ��� ���������� �������
		Timer timerTail;
		//���� ����������� ��� ��� ���������� ������� � ������ ������
		volatile bool calcTailsWorking = false;
		//������ ��� �������������: ���������� �������
		object obSyncCalcTails = new object();
		//���� ����������� ��� ������ ���� ���������
		bool calcTailesMode = true;

		//������ ��� �������������: ���������� �������, ������� �������, ��������� ����
		object obLock = new object();

		//������� ����������
		int frameNumber;
		public float frameRate;
		DateTime frameRateTime;*/

		RLGM::ReturnCode CreateDevice();
		RLGM::ReturnCode LoadShaders();
		HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

		Interface();
		~Interface();

		//��������� �������������/���������������
		RLGM::ReturnCode Init(int tails_calc_period_ms);
		void DeInit();
		//�������, ������� ����
		RLGM::ReturnCode CreateView(HWND hWnd, int nViewId);
		void DeleteView(int nViewId);
		//�������, ������� ����� (should be >0)
		RLGM::ReturnCode CreateRadar(int nRadarId); //nRadarId should be >0
		void DeleteRadar(int nRadarId);
		//�������� ���� � �����
		RLGM::ReturnCode AddPRLI(int nRadarId, RLGM::PRLIType nType, int nPRLISampleCount, int nPRLILineCount);

		//���������� �������
		void CalcTails(int period_ms);

		//���������������� ������: �����
		void SetRadarPos(int nViewId, int nRadarId, int nX, int nY);
		std::pair<int, int> GetRadarPos(int nViewId, int nRadarId);
		void AddRadarPos(int nViewId, int nRadarId, int nDX, int nDY);
		//���������������� ������: �������
		void SetDPP(int nViewId, int nRadarId, float dDPP);
		float GetDPP(int nViewId, int nRadarId);
		void MultiplyDPP(int nViewId, int nRadarId, float value);
		void MultiplyDPPCenter(int nViewId, int nRadarId, float value);
		//���������������� ������: �������
		void SetRotation(int nViewId, int nRadarId, float dRotInGrad);
		void AddRotation(int nViewId, int nRadarId, float delta);

		//void RestoreDevice();

		//���������� ����
		RLGM::ReturnCode DrawView(int nViewId);

		//��������� ��������������
		void SetPRLIColorCorrection(int nViewId, int nRadarId, RLGM::PRLIType nPRLIType, int nCount, uint32_t* pPalette);

		//��������� ����-�� �������
		void SetUnificationCoefs(int nRadarId, const std::vector<uint16_t>& values);

		//��������� ����������� ����
		void SetShowPRLI(int nViewId, int nRadarId, RLGM::PRLIType nPRLIType, bool nShow);
		bool GetShowPRLI(int nViewId, int nRadarId, RLGM::PRLIType nPRLIType);

		//�������� ������� 8 � 2 �����
		void Set8bitData(int nRadarId, int start_line, int stop_line, const std::vector<float> &data, int offset);
		void Set2bitData(int nRadarId, int start_line, int stop_line, const std::vector<uint8_t> &data, int offset);
	
		//������� ����������������� ����
		ID3D11Texture2D* GetMapLayer(int nViewId);
		void ReleaseMapLayer(int nViewId);

		//��������� ����� ���������������
		void SetTextureSamplerType(int nViewId, RLGM::TextureSamplerType value);
		RLGM::TextureSamplerType GetTextureSamplerType(int nViewId);

		//��������� ����������� ����
		void SetShowMap(int nViewId, bool nShow);
		void SetShow8bit(int nViewId, bool nShow);
		void SetShow2bit(int nViewId, bool nShow);
		void SetShowTails(int nViewId, bool nShow);
		bool GetShowMap(int nViewId);
		bool GetShow8bit(int nViewId);
		bool GetShow2bit(int nViewId);
		bool GetShowTails(int nViewId);
		
		//�������� ������
		void ClearTailes(int nRadarId);

		//���������� ������� ������� 8 �����
		void Set8bitUnificationRule(int nViewId, RLGM::PRLIUnificationRule value);
		RLGM::PRLIUnificationRule Get8bitUnificationRule(int nViewId);

		//���������������� ���������� ����
		void SetSurfacesShift(int nViewId, int nx, int ny);
		void AddSurfacesShift(int nViewId, int dx, int dy);
		void MultiplySurfacesDPP(int nViewId, float value);
		void MultiplySurfacesDPPCenter(int nViewId, float value);

		//����� 2����� ��� ���������� ������
		void SetThr2BitForTails(int value);
	};
};


//������� �������� ���� 2
//public IntPtr GetTopSurface2(int nViewId);
//public void ReleaseTopSurface2(int nViewId)

//������� �������� ���� 1
//public IntPtr GetTopSurface1(int nViewId)
//public IntPtr GetTopSurface1HDC(int nViewId)
//public void ReleaseTopSurface1(int nViewId)
//public void ReleaseTopSurface1HDC(int nViewId, IntPtr hDC);
//�������� ������� ���� 1
//public void ClearTop1(int nViewId)
//�������� ������� ���� 2
//public void ClearTop2(int nViewId)

//���������� ������� ����
//public void SetBackColor(int nViewId, int colorARGBA)

//��������� ����������� �������� ���� 1
//public void SetShowTop1(int nViewId, int nShow)
//public bool GetShowTop1(int nViewId, int nShow)

//��������� ����������� �������� ���� 2
//public void SetShowTop2(int nViewId, int nShow)
//public bool GetShowTop2(int nViewId, int nShow)

//��������� ����������� ��������
//public void SetShowPictures(int nViewId, int nShow)
//public bool GetShowPictures(int nViewId)

//������� ��� ������� ���� ��� ���������� ������
//public void SetTimeoutForClearing(int nRadarId, int nMseconds)

//���������� ������� ������� 2 �����
//public void Set2bitIntegrationRule(int nViewId, int nRule)
//public void Set2bitIntegrationRule(int nViewId, IntegrationRuleType rule)

//��������� ����� ����������� ������
//public void SetRadarDrawMode(int nViewId, int nMode)

//��������� ��������
//public void SetPicture(int nPictureId, string bstrFileName, out uint width, out uint height)
//���������������� ��������: �����
//public void SetPicturePos(int nViewId, int nX, int nY, int nPictureId)
//������� ��������
//public void DeletePicture(int nViewId, int nPictureId)


//��������� �������
//if (drawPictures)
//{
//	owner.sprite.Begin(SpriteFlags.AlphaBlend);
//	foreach(var item in((Hashtable)(picturesPosition.Clone())).Keys)
//		owner.sprite.Draw((Texture)owner.pictures[item], new Vector3(((Texture)(owner.pictures[item])).GetLevelDescription(0).Width / 2, ((Texture)(owner.pictures[item])).GetLevelDescription(0).Height / 2, 0), new Vector3(((Point)(picturesPosition[item])).X + picturesPositionOffset.X, ((Point)(picturesPosition[item])).Y + picturesPositionOffset.Y, 0), new Color4(1, 1, 1, 1));
//	owner.sprite.End();
//}
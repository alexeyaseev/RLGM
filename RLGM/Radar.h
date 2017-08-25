#pragma once

#include "help.h"

namespace RLGM {
	class Interface;
}

namespace RLGM 
{
	class Radar
	{
	public:
		RLGM::Interface* owner = nullptr;

		//id радара
		int id = -1;
		double m_height = 0;
		double m_longitude = 0;
		double m_latitude = 0;
		double m_sampleSize = 1;
		double m_angle = 0;

		//количество линеек и точек дальности
		int numLines = -1;
	public:
		int numSamples = -1;
	private:

		//8 битка
		RLGM::Texture texture8Bit;

		//2 битка
		RLGM::Texture texture2Bit;

		//Хвосты
		RLGM::Texture textureTail[2];
		int textureTailIndexForCalc = 0; //индекс формируемого хвоста
		
		//таблица коэффициентов склейки ПРЛИ
		RLGM::Texture textureUnificationCoefs;

		//Геометрия развёртки
		RLGM::PositionedTexturedVertex* vertexes = nullptr;
		ID3D11Buffer* vertexBuffer = nullptr;

		//Геометрия для вычисления хвостов
		//static PositionedTexturedVertex[] vertexesTail;
		//static VertexBuffer vertexBufferTail;

		//номера линеек: последняя обновлённая линейка в оперативной памяти, последняя обновлённая линейка в памяти видеокарты
		//int lastReceivedLine = -1;
		//int lastUpdatedLine = -1;


		//время последнего обновления ПРЛИ
		/*DateTime LastTimeWhenPRLIUpdated = DateTime.MinValue;
		//таймаут по истечении которого считаем что ПРЛИ устарела
		int timeout = 5000;
		public int Timeout
		{
			get
		{
			return timeout;
		}
			set
		{
			timeout = value;
		}
		}*/


	public:
		Radar();
		Radar(int Id, double height, double longitude, double latitude, RLGM::Interface* Owner);
		//Добавить ПРЛИ
		void AddPRLI(RLGM::PRLIType nType, int nPRLISampleCount, int nPRLILineCount);
		//Обновление линейки ПРЛИ в памяти видеокарты
		void Set8bitData(int start_line, int stop_line, const std::vector<float> &data8bit, int offset);
		void Set8bitData(int start_line, int stop_line, const std::vector<uint8_t> &data8bit, int offset);
		void Set2bitData(int start_line, int stop_line, const std::vector<uint8_t> &data8bit, int offset);

		DirectX::XMMATRIX Radar::CalculateWorldViewProjMatrix(RLGM::GeoCoord view_position, double view_aspect, double view_scale);
		//Отрисовка 8 битки
		//void Draw8bit(RLGM::RadarPositioning geom, float aspect, ID3D11PixelShader* ps, ID3D11VertexShader* vs, ID3D11SamplerState* sampler/*, RadarDrawMode drawMode*/);
		void Draw8bit(RLGM::GeoCoord view_position, double aspect, double scale, ID3D11PixelShader* ps, ID3D11VertexShader* vs, ID3D11SamplerState* sampler/*, RadarDrawMode drawMode*/);
		//Отрисовка 2 битки
		void Draw2bit(RLGM::GeoCoord view_position, double view_aspect, double view_scale, ID3D11PixelShader* ps, ID3D11VertexShader* vs, ID3D11SamplerState* sampler/*, RadarDrawMode drawMode*/);
		void Draw2bitTail(RLGM::GeoCoord view_position, double view_aspect, double view_scale, ID3D11PixelShader* ps, ID3D11VertexShader* vs, ID3D11SamplerState* sampler/*, RadarDrawMode drawMode*/);
		void DrawTail(RLGM::GeoCoord view_position, double view_aspect, double view_scale, ID3D11PixelShader* ps, ID3D11VertexShader* vs, ID3D11SamplerState* sampler/*, RadarDrawMode drawMode*/);


		void SetUnificationCoefs(const std::vector<uint16_t>& values);

		void CalcTails(ID3D11PixelShader* ps, ID3D11VertexShader* vs, int thr_2_bit);
		void ClearTails();

		~Radar();
	};
};


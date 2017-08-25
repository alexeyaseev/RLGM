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

		//id ������
		int id = -1;
		double m_height = 0;
		double m_longitude = 0;
		double m_latitude = 0;
		double m_sampleSize = 1;
		double m_angle = 0;

		//���������� ������ � ����� ���������
		int numLines = -1;
	public:
		int numSamples = -1;
	private:

		//8 �����
		RLGM::Texture texture8Bit;

		//2 �����
		RLGM::Texture texture2Bit;

		//������
		RLGM::Texture textureTail[2];
		int textureTailIndexForCalc = 0; //������ ������������ ������
		
		//������� ������������� ������� ����
		RLGM::Texture textureUnificationCoefs;

		//��������� ��������
		RLGM::PositionedTexturedVertex* vertexes = nullptr;
		ID3D11Buffer* vertexBuffer = nullptr;

		//��������� ��� ���������� �������
		//static PositionedTexturedVertex[] vertexesTail;
		//static VertexBuffer vertexBufferTail;

		//������ ������: ��������� ���������� ������� � ����������� ������, ��������� ���������� ������� � ������ ����������
		//int lastReceivedLine = -1;
		//int lastUpdatedLine = -1;


		//����� ���������� ���������� ����
		/*DateTime LastTimeWhenPRLIUpdated = DateTime.MinValue;
		//������� �� ��������� �������� ������� ��� ���� ��������
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
		//�������� ����
		void AddPRLI(RLGM::PRLIType nType, int nPRLISampleCount, int nPRLILineCount);
		//���������� ������� ���� � ������ ����������
		void Set8bitData(int start_line, int stop_line, const std::vector<float> &data8bit, int offset);
		void Set8bitData(int start_line, int stop_line, const std::vector<uint8_t> &data8bit, int offset);
		void Set2bitData(int start_line, int stop_line, const std::vector<uint8_t> &data8bit, int offset);

		DirectX::XMMATRIX Radar::CalculateWorldViewProjMatrix(RLGM::GeoCoord view_position, double view_aspect, double view_scale);
		//��������� 8 �����
		//void Draw8bit(RLGM::RadarPositioning geom, float aspect, ID3D11PixelShader* ps, ID3D11VertexShader* vs, ID3D11SamplerState* sampler/*, RadarDrawMode drawMode*/);
		void Draw8bit(RLGM::GeoCoord view_position, double aspect, double scale, ID3D11PixelShader* ps, ID3D11VertexShader* vs, ID3D11SamplerState* sampler/*, RadarDrawMode drawMode*/);
		//��������� 2 �����
		void Draw2bit(RLGM::GeoCoord view_position, double view_aspect, double view_scale, ID3D11PixelShader* ps, ID3D11VertexShader* vs, ID3D11SamplerState* sampler/*, RadarDrawMode drawMode*/);
		void Draw2bitTail(RLGM::GeoCoord view_position, double view_aspect, double view_scale, ID3D11PixelShader* ps, ID3D11VertexShader* vs, ID3D11SamplerState* sampler/*, RadarDrawMode drawMode*/);
		void DrawTail(RLGM::GeoCoord view_position, double view_aspect, double view_scale, ID3D11PixelShader* ps, ID3D11VertexShader* vs, ID3D11SamplerState* sampler/*, RadarDrawMode drawMode*/);


		void SetUnificationCoefs(const std::vector<uint16_t>& values);

		void CalcTails(ID3D11PixelShader* ps, ID3D11VertexShader* vs, int thr_2_bit);
		void ClearTails();

		~Radar();
	};
};


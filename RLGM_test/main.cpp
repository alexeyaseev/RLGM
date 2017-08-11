#include <map>
#include <array>
#include "ViewForm.h"
#include "ControlPanel.h"
#include "ColorCorrection8bit.h"
#include "ColorCorrection2bit.h"
#include "ColorCorrectionTails.h"
#include "UnificationCoef.h"

#include <QtWidgets/QApplication>

#include "../../RLGM/Interface.h"

uint32_t RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

//---> 8 bit Color correction default params
const double DEFAULT_LOW_THR_COLORCORRECTION_8BIT = 70 / 255.0;
const double DEFAULT_HIGH_THR_COLORCORRECTION_8BIT = 180 / 255.0;
const double DEFAULT_MIN_LUMA_COLORCORRECTION_8BIT = 0 / 255.0;
const double DEFAULT_HUE_COLORCORRECTION_8BIT = 1/6.0;
const double DEFAULT_SAT_COLORCORRECTION_8BIT = 1.0;
const double DEFAULT_GAMMA_COLORCORRECTION_8BIT = 0.7; // in [0,1] range, where 0 = 1/MAX_GAMMA, 0.5 = 1, 1 = MAX_GAMMA
const double DEFAULT_MAX_GAMMA_COLORCORRECTION_8BIT = 1.5;
const double DEFAULT_ALPHA_THR_COLORCORRECTION_8BIT = 1.0;
//<--- 8 bit Color correction
//---> 2 bit Color correction default params
const uint32_t DEFAULT_2BIT_COLOR1 = RGBA(0, 0, 0, 0);
const uint32_t DEFAULT_2BIT_COLOR2 = RGBA(204, 119, 34, 255);
const uint32_t DEFAULT_2BIT_COLOR3 = RGBA(255, 160, 0, 255);
const uint32_t DEFAULT_2BIT_COLOR4 = RGBA(255, 255, 0, 255);
//const uint32_t DEFAULT_2BIT_COLOR2 = RGBA(255, 175, 175, 255);
//const uint32_t DEFAULT_2BIT_COLOR3 = RGBA(255, 135, 135, 255);
//const uint32_t DEFAULT_2BIT_COLOR4 = RGBA(255, 0, 0, 255);
//<--- 2 bit Color correction
//---> Tail Color correction default params
const double DEFAULT_LOW_THR_COLORCORRECTION_TAIL = 0.5;
const double DEFAULT_MIN_LUMA_COLORCORRECTION_TAIL = 80 / 255.0;
const double DEFAULT_HUE_COLORCORRECTION_TAIL = 2 / 6.0;
const double DEFAULT_SAT_COLORCORRECTION_TAIL = 0.0;
//<--- Tail Color correction


//---> Unification coefs
const double DEFAULT_START_UNIFICATION_VALUE = 0.5;
const double DEFAULT_STOP_UNIFICATION_VALUE = 0.5;
//<--- Unification coefs

RLGM::Interface moduleRLGM; //RLGM module
bool g_bExit = false; //application exit flag
bool g_bPause = false;
int sweepTime_ms = 3000;
std::map<std::pair<int, int>, ColorCorrection8bit> colorCorrections8bit;
std::map<std::pair<int, int>, ColorCorrection2bit> colorCorrections2bit;
std::map<std::pair<int, int>, ColorCorrectionTails> colorCorrectionsTails;
std::map<int, UnificationCoef> unificationCoefs;
ControlPanel* moduleControlPanel = nullptr;
std::vector<ViewForm*> viewForms; //RLGM::View forms
int calcTailsPeriod_ms = 1000; //tails calculation period

std::thread drawThread;
static int drawPeriod_ms = 17; //1000/17 = 60 fps

void DrawThread() {
	while (!g_bExit) {
		moduleRLGM.DrawView(-1);
		Sleep(drawPeriod_ms); // to have about 60 fps
	}
}
void DataThreadRawenstvo(int sweep_time_ms)
{
	int numLines = 8192;
	int numSamples = 8192;

	std::vector<uint8_t> line2bit_1(numSamples * numLines);
	std::vector<uint8_t> line8bit_1(numSamples * numLines);
	std::vector<uint8_t> line2bit_2(numSamples * numLines);
	std::vector<uint8_t> line8bit_2(numSamples * numLines);
	std::vector<float> line8bitFloat_1(numSamples * numLines);
	std::vector<float> line8bitFloat_2(numSamples * numLines);

	int targetsweeptime_ms = sweep_time_ms;
	int minsleep_ms = 20;
	int sleepbatch = (int)(0.5f + minsleep_ms * numLines / (float)targetsweeptime_ms);
	int pushbatch = 1; //should be power of 2

	std::ifstream ifs8bit_1, ifs8bit_2;
	//ifs8bit_2.open("../../records/rawenstvo_40.bt8", std::ios::binary);
	ifs8bit_1.open("../../records/rawenstvo.bt8", std::ios::binary);
	
	//ifs8bit_2.open("../../records/rawenstvo_no_noise_40.bt8", std::ios::binary);
	//ifs8bit_1.open("../../records/rawenstvo_no_noise.bt8", std::ios::binary);

	auto start = std::chrono::steady_clock::now();
	int line = 0;
	int sweep = 0;

	//int aim_line = 0; //for tail test

	while (!g_bExit)
	{
		if (g_bPause) continue;

		ifs8bit_1.read((char*)&line8bit_1[line * numSamples], numSamples);
		if (ifs8bit_1.eof()) {
			ifs8bit_1.clear();
			ifs8bit_1.seekg(0, std::ios::beg);
		}
		//ifs8bit_2.read((char*)&line8bit_2[line * numSamples], numSamples);
		//if (ifs8bit_2.eof()) {
		//	ifs8bit_2.clear();
		//	ifs8bit_2.seekg(0, std::ios::beg);
		//}

		//calculate 2 bit
		int sum = 0;
		for (int i = 0; i < numSamples; i++)
			sum += line8bit_1[line * numSamples + i];
		float mean = sum / numSamples;
		for (int i = 0; i < numSamples; i++)
			if (line8bit_1[line * numSamples + i] > 5 * mean)
				line2bit_1[line * numSamples + i] = 3;
			else
				line2bit_1[line * numSamples + i] = 0;
		//sum = 0;
		//for (int i = 0; i < numSamples; i++)
		//	sum += line8bit_2[line * numSamples + i];
		//mean = sum / numSamples;
		//for (int i = 0; i < numSamples; i++)
		//	if (line8bit_2[line * numSamples + i] > 5 * mean)
		//		line2bit_2[line * numSamples + i] = 3;
		//	else
		//		line2bit_2[line * numSamples + i] = 0;

		// fot tail test
		/*if (line == aim_line) {
		for (int k = 1000; k <= 1000 + 10; k++)
		line2bit[k] = 3;
		aim_line -= 1;
		if (aim_line < 0) aim_line = numLines - 1;
		}*/

		for (int i = 0; i < numSamples; i++) line8bitFloat_1[line * numSamples + i] = line8bit_1[line * numSamples + i] / 255.0f;
		for (int i = 0; i < numSamples; i++) line8bitFloat_2[line * numSamples + i] = line8bit_2[line * numSamples + i] / 255.0f;

		if (line % pushbatch == pushbatch - 1) {
			moduleRLGM.Set8bitData(1, line - pushbatch + 1, line, line8bitFloat_1, (line - pushbatch + 1) * numSamples);
			moduleRLGM.Set2bitData(1, line - pushbatch + 1, line, line2bit_1, (line - pushbatch + 1) * numSamples);
			//moduleRLGM.Set8bitData(2, line - pushbatch + 1, line, line8bit_2, (line - pushbatch + 1) * numSamples);
			//moduleRLGM.Set2bitData(2, line - pushbatch + 1, line, line2bit_2, (line - pushbatch + 1) * numSamples);
		}

		if (line % sleepbatch == 0)
			std::this_thread::sleep_for(std::chrono::milliseconds(minsleep_ms));

		line = (line + 1) % numLines;
		if (line == 0) {
			sweep++;
			auto stop = std::chrono::steady_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (stop - start).count();
			start = std::chrono::steady_clock::now();
			std::cout << "Sweep time: " << duration << std::endl;
		}
	}
}

void DataThreadMkis(int sweep_time_ms)
{	
	int numLines = 4096;
	int numSamples = 4096;

	std::vector<uint8_t> line2bit(numSamples * numLines);
	std::vector<uint8_t> line8bit(numSamples * numLines);
	std::vector<float> line8bitFloat(numSamples * numLines);

	int targetsweeptime_ms = sweep_time_ms;
	int minsleep_ms = 20;
	int sleepbatch = (int)(0.5f + minsleep_ms * numLines / (float)targetsweeptime_ms);
	int pushbatch = 2; //should be power of 2

	std::ifstream ifs8bit, ifs2bit;
	ifs8bit.open("../../records/mkis.bt8", std::ios::binary);
	ifs2bit.open("../../records/mkis.bt2", std::ios::binary);

	auto start = std::chrono::steady_clock::now();
	int line = 0;
	int sweep = 0;

	//int aim_line = 0; //for tail test

	while (!g_bExit)
	{
		if (g_bPause) continue;

		ifs8bit.read((char*)&line8bit[line * numSamples], numSamples);
		if (ifs8bit.eof()) {
			ifs8bit.clear();
			ifs8bit.seekg(0, std::ios::beg);
		}
		ifs2bit.read((char*)&line2bit[line * numSamples], numSamples);
		if (ifs2bit.eof()) {
			ifs2bit.clear();
			ifs2bit.seekg(0, std::ios::beg);
		}

		// fot tail test
		/*if (line == aim_line) {
			for (int k = 1000; k <= 1000 + 10; k++)
				line2bit[k] = 3;
			aim_line -= 1;
			if (aim_line < 0) aim_line = numLines - 1;
		}*/
		for (int i = 0; i < numSamples; i++)
			line8bitFloat[line * numSamples + i] = line8bit[line * numSamples + i] / 255.0f;

		if (line % pushbatch == pushbatch - 1) {
			moduleRLGM.Set8bitData(-1, line - pushbatch + 1, line, line8bitFloat, (line - pushbatch + 1) * numSamples);
			moduleRLGM.Set2bitData(-1, line - pushbatch + 1, line, line2bit, (line - pushbatch + 1) * numSamples);
		}

		if (line % sleepbatch == 0)
			std::this_thread::sleep_for(std::chrono::milliseconds(minsleep_ms));

		line = (line + 1) % numLines;
		if (line == 0) {
			sweep++;
			auto stop = std::chrono::steady_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (stop - start).count();
			start = std::chrono::steady_clock::now();
			std::cout << "Sweep time: " << duration << std::endl;
		}
	}
}

int mainMkis(int argc, char *argv[]) //MKIS
{
	QApplication app(argc, argv);

	//---> attaching Console
	// detach from the current console window
	// if launched from a console window, that will still run waiting for the new console (below) to close
	// it is useful to detach from Qt Creator's <Application output> panel
	FreeConsole();
	// create a separate new console window
	AllocConsole();
	// attach the new console to this application's process
	AttachConsole(GetCurrentProcessId());
	// reopen the std I/O streams to redirect I/O to the new console
	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);
	freopen("CON", "r", stdin);
	//<--- attaching Console

	int numLines = 4096;
	int numSamples = 4096;
	int radarsPerRow = 2;
	int radarsPerCol = 1;
	int numViews = 1;
	std::thread dataThread;

	//---> creation of RLGM module
	moduleRLGM.Init(calcTailsPeriod_ms);

	//creation of RLGM::Views
	for (int v = 1; v <= numViews; v++)
	{
		ViewForm* form = new ViewForm(v >= 2, &moduleRLGM, v);
		viewForms.push_back(form);
		viewForms[v-1]->show();
		moduleRLGM.CreateView((HWND)viewForms[v-1]->winId(), v);
	}
	
	//adding map
	IDXGISurface1* pIDXGSurface = nullptr;
	moduleRLGM.GetMapLayer(1)->QueryInterface(__uuidof(IDXGISurface1), reinterpret_cast<void**>(&pIDXGSurface));
	HDC hDc = nullptr;
	HRESULT hr = pIDXGSurface->GetDC(true, &hDc);
	HINSTANCE hInst = GetModuleHandle(NULL);
	HBITMAP hBitmap = (HBITMAP)LoadImage(hInst, L"map.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HDC hdcMem = CreateCompatibleDC(hDc);
	HGDIOBJ oldBitmap = SelectObject(hdcMem, hBitmap);
	BITMAP          bitmap;
	GetObject(hBitmap, sizeof(bitmap), &bitmap);
	BitBlt(hDc, 0, 0, min(moduleRLGM.views.at(1).width,bitmap.bmWidth), min(moduleRLGM.views.at(1).height, bitmap.bmHeight), hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, oldBitmap);
	DeleteDC(hdcMem);
	hr = pIDXGSurface->ReleaseDC(nullptr);
	moduleRLGM.ReleaseMapLayer(1);

	//creation of RLGM::Radars
	for (int r = 1; r <= radarsPerRow * radarsPerCol; r++) //avoid using zero radar id
	{
		moduleRLGM.CreateRadar(r);
		moduleRLGM.AddPRLI(r, RLGM::PRLIType::PRLI_8bit, numSamples, numLines);
		moduleRLGM.AddPRLI(r, RLGM::PRLIType::PRLI_2bit, numSamples, numLines);
		unificationCoefs.insert(std::make_pair(r, UnificationCoef(numSamples, DEFAULT_START_UNIFICATION_VALUE, DEFAULT_STOP_UNIFICATION_VALUE)));
		moduleRLGM.SetUnificationCoefs(r, unificationCoefs.at(r).m_coefs);
		for (int v = 1; v <= numViews; v++)
		{
			//set color correction
			std::pair<int, int> key(v, r);
			colorCorrections8bit.insert(std::make_pair(key, ColorCorrection8bit(DEFAULT_LOW_THR_COLORCORRECTION_8BIT,
				DEFAULT_HIGH_THR_COLORCORRECTION_8BIT,
				DEFAULT_GAMMA_COLORCORRECTION_8BIT,
				DEFAULT_HUE_COLORCORRECTION_8BIT,
				DEFAULT_SAT_COLORCORRECTION_8BIT,
				DEFAULT_MIN_LUMA_COLORCORRECTION_8BIT,
				DEFAULT_MAX_GAMMA_COLORCORRECTION_8BIT,
				DEFAULT_ALPHA_THR_COLORCORRECTION_8BIT)));
			moduleRLGM.SetPRLIColorCorrection(v, r, RLGM::PRLIType::PRLI_8bit, 256, colorCorrections8bit.at(key).colorsRGBA);
			colorCorrections2bit.insert(std::make_pair(key, ColorCorrection2bit(DEFAULT_2BIT_COLOR1, DEFAULT_2BIT_COLOR2, DEFAULT_2BIT_COLOR3, DEFAULT_2BIT_COLOR4)));
			moduleRLGM.SetPRLIColorCorrection(v, r, RLGM::PRLIType::PRLI_2bit, 256, colorCorrections2bit.at(key).colorsRGBA);
			colorCorrectionsTails.insert(std::make_pair(key, ColorCorrectionTails(DEFAULT_LOW_THR_COLORCORRECTION_TAIL,
				DEFAULT_MIN_LUMA_COLORCORRECTION_TAIL,
				DEFAULT_HUE_COLORCORRECTION_TAIL,
				DEFAULT_SAT_COLORCORRECTION_TAIL)));
			moduleRLGM.SetPRLIColorCorrection(v, r, RLGM::PRLIType::PRLI_Tail, 256, colorCorrectionsTails.at(key).colorsRGBA);

			//set radar positions (table layout)
			RECT rc;
			GetClientRect((HWND)viewForms[v-1]->winId(), &rc);
			int width  = rc.right - rc.left;
			int height = rc.bottom - rc.top;
			moduleRLGM.SetRadarPos(v, r, (r % (radarsPerRow + 1)) * width / (radarsPerRow + 1), (r / (radarsPerRow + 1) + 1) * height / (radarsPerCol + 1));
		}
	}
	//<--- creation of RLGM module

	//creation of Control Panel
	moduleControlPanel = new ControlPanel(&moduleRLGM, &colorCorrections8bit, &colorCorrections2bit, &colorCorrectionsTails, &unificationCoefs, &app, &g_bPause);
	moduleControlPanel->setWindowFlags(Qt::WindowStaysOnTopHint);
	moduleControlPanel->show();
	
	//data and draw thread creation
	dataThread = std::thread(DataThreadMkis, sweepTime_ms);
	drawThread = std::thread(DrawThread);

	//entering into window message cycle
	app.exec();
	
	//application exit
	g_bExit = true;
	dataThread.join();
	drawThread.join();
	moduleRLGM.DeInit();
	
	return 0;
}

int mainRawenstvo(int argc, char *argv[]) 
{
	QApplication app(argc, argv);

	//---> attaching Console
	// detach from the current console window
	// if launched from a console window, that will still run waiting for the new console (below) to close
	// it is useful to detach from Qt Creator's <Application output> panel
	FreeConsole();
	// create a separate new console window
	AllocConsole();
	// attach the new console to this application's process
	AttachConsole(GetCurrentProcessId());
	// reopen the std I/O streams to redirect I/O to the new console
	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);
	freopen("CON", "r", stdin);
	//<--- attaching Console

	int numLines = 8192;
	int numSamples = 8192;
	int radarsPerRow = 1;
	int radarsPerCol = 1;
	int numViews = 1;
	std::thread dataThread;

	//---> creation of RLGM module
	moduleRLGM.Init(calcTailsPeriod_ms);

	//creation of RLGM::Views
	for (int v = 1; v <= numViews; v++)
	{
		ViewForm* form = new ViewForm(v >= 2, &moduleRLGM, v);
		viewForms.push_back(form);
		viewForms[v - 1]->show();
		moduleRLGM.CreateView((HWND)viewForms[v - 1]->winId(), v);
	}

	//adding map
	IDXGISurface1* pIDXGSurface = nullptr;
	moduleRLGM.GetMapLayer(1)->QueryInterface(__uuidof(IDXGISurface1), reinterpret_cast<void**>(&pIDXGSurface));
	HDC hDc = nullptr;
	HRESULT hr = pIDXGSurface->GetDC(true, &hDc);
	HINSTANCE hInst = GetModuleHandle(NULL);
	HBITMAP hBitmap = (HBITMAP)LoadImage(hInst, L"map.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HDC hdcMem = CreateCompatibleDC(hDc);
	HGDIOBJ oldBitmap = SelectObject(hdcMem, hBitmap);
	BITMAP          bitmap;
	GetObject(hBitmap, sizeof(bitmap), &bitmap);
	BitBlt(hDc, 0, 0, min(moduleRLGM.views.at(1).width, bitmap.bmWidth), min(moduleRLGM.views.at(1).height, bitmap.bmHeight), hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, oldBitmap);
	DeleteDC(hdcMem);
	hr = pIDXGSurface->ReleaseDC(nullptr);
	moduleRLGM.ReleaseMapLayer(1);

	//creation of RLGM::Radars
	for (int r = 1; r <= radarsPerRow * radarsPerCol; r++) //avoid using zero radar id
	{
		moduleRLGM.CreateRadar(r);
		moduleRLGM.AddPRLI(r, RLGM::PRLIType::PRLI_8bit, numSamples, numLines);
		moduleRLGM.AddPRLI(r, RLGM::PRLIType::PRLI_2bit, numSamples, numLines);
		unificationCoefs.insert(std::make_pair(r, UnificationCoef(numSamples, DEFAULT_START_UNIFICATION_VALUE, DEFAULT_STOP_UNIFICATION_VALUE)));
		moduleRLGM.SetUnificationCoefs(r, unificationCoefs.at(r).m_coefs);
		for (int v = 1; v <= numViews; v++)
		{
			//set color correction
			std::pair<int, int> key(v, r);
			colorCorrections8bit.insert(std::make_pair(key, ColorCorrection8bit(DEFAULT_LOW_THR_COLORCORRECTION_8BIT,
				DEFAULT_HIGH_THR_COLORCORRECTION_8BIT,
				DEFAULT_GAMMA_COLORCORRECTION_8BIT,
				DEFAULT_HUE_COLORCORRECTION_8BIT,
				DEFAULT_SAT_COLORCORRECTION_8BIT,
				DEFAULT_MIN_LUMA_COLORCORRECTION_8BIT,
				DEFAULT_MAX_GAMMA_COLORCORRECTION_8BIT,
				DEFAULT_ALPHA_THR_COLORCORRECTION_8BIT)));
			moduleRLGM.SetPRLIColorCorrection(v, r, RLGM::PRLIType::PRLI_8bit, 256, colorCorrections8bit.at(key).colorsRGBA);
			colorCorrections2bit.insert(std::make_pair(key, ColorCorrection2bit(DEFAULT_2BIT_COLOR1, DEFAULT_2BIT_COLOR2, DEFAULT_2BIT_COLOR3, DEFAULT_2BIT_COLOR4)));
			moduleRLGM.SetPRLIColorCorrection(v, r, RLGM::PRLIType::PRLI_2bit, 256, colorCorrections2bit.at(key).colorsRGBA);
			colorCorrectionsTails.insert(std::make_pair(key, ColorCorrectionTails(DEFAULT_LOW_THR_COLORCORRECTION_TAIL,
				DEFAULT_MIN_LUMA_COLORCORRECTION_TAIL,
				DEFAULT_HUE_COLORCORRECTION_TAIL,
				DEFAULT_SAT_COLORCORRECTION_TAIL)));
			moduleRLGM.SetPRLIColorCorrection(v, r, RLGM::PRLIType::PRLI_Tail, 256, colorCorrectionsTails.at(key).colorsRGBA);

			//set radar positions (table layout)
			//RECT rc;
			//GetClientRect((HWND)viewForms[v - 1]->winId(), &rc);
			//int width = rc.right - rc.left;
			//int height = rc.bottom - rc.top;
			//moduleRLGM.SetRadarPos(v, r, (r % (radarsPerRow + 1)) * width / (radarsPerRow + 1), (r / (radarsPerRow + 1) + 1) * height / (radarsPerCol + 1));
		}
	}
	//<--- creation of RLGM module

	//creation of Control Panel
	moduleControlPanel = new ControlPanel(&moduleRLGM, &colorCorrections8bit, &colorCorrections2bit, &colorCorrectionsTails, &unificationCoefs, &app, &g_bPause);
	moduleControlPanel->setWindowFlags(Qt::WindowStaysOnTopHint);
	moduleControlPanel->show();

	//data and draw thread creation
	dataThread = std::thread(DataThreadRawenstvo, sweepTime_ms);
	drawThread = std::thread(DrawThread);

	//entering into window message cycle
	app.exec();

	//application exit
	g_bExit = true;
	dataThread.join();
	drawThread.join();
	moduleRLGM.DeInit();

	return 0;
}

int main(int argc, char *argv[])
{
	return mainMkis(argc, argv);
	//return mainRawenstvo(argc, argv);
}



uint32_t RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	return a << 24 | b << 16 | g << 8 | r;
}
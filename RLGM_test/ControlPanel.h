#pragma once

#include <QWidget>
#include "ui_ControlPanel.h"

#include "../../RLGM/Interface.h"

#include "ColorCorrection8bit.h"
#include "ColorCorrection2bit.h"
#include "ColorCorrectionTails.h"
#include "UnificationCoef.h"

class ControlPanel : public QWidget
{
	Q_OBJECT

	RLGM::Interface* moduleRLGM = nullptr;
	QApplication* ownerApp = nullptr;
	std::map<std::pair<int, int>, ColorCorrection8bit>* colorCorrections8bit  = nullptr;
	std::map<std::pair<int, int>, ColorCorrection2bit>* colorCorrections2bit  = nullptr;
	std::map<std::pair<int, int>, ColorCorrectionTails>* colorCorrectionsTails = nullptr;
	std::map<int, UnificationCoef>* unificationCoefs = nullptr;
	bool* pause;

public:
	ControlPanel(RLGM::Interface* rlgm, 
				 std::map<std::pair<int, int>, ColorCorrection8bit>* color_corrections_8bit, 
				 std::map<std::pair<int, int>, ColorCorrection2bit>* color_corrections_2bit, 
				 std::map<std::pair<int, int>, ColorCorrectionTails>* color_corrections_Tails,
				 std::map<int, UnificationCoef>* unification_coefs,
				 QApplication* owner,
				 bool* pause_flag,
				 QWidget *parent = Q_NULLPTR);
	~ControlPanel();

private:
	Ui::ControlPanel ui;
	
	void UpdateControls();
	int GetViewId();
	int GetRadarId();

private slots:

	void clearTailsHandler();
	void buttonExitHandler();

	void button2bitColor1Handler();
	void button2bitColor2Handler();
	void button2bitColor3Handler();

	void viewChangedHandler(int);
	void radarChangedHandler(int);
	void lowThr8bitChanged(int);
	void highThr8bitChanged(int);
	void gamma8bitChanged(int);
	void hue8bitChanged(int);
	void sat8bitChanged(int);
	void startValueUnification8bitChanged(int);
	void stopValueUnification8bitChanged(int);
	void draw8bitFlagChanged(bool);

	void draw2bitFlagChanged(bool);
	void drawTailsFlagChanged(bool);

	void draw8bitLayerFlagChanged(bool);
	void draw2bitLayerFlagChanged(bool);
	void drawMapLayerFlagChanged(bool);
	void drawTailsLayerFlagChanged(bool);



	void textureSamplerTypeChanged(bool);
	void unification8bitRuleChanged(bool);

	void alphaThr8bitChanged(int);

	void tailSizeChanged(int);
	void tailMinLumaChanged(int);
	void tailHueChanged(int);
	void tailSatChanged(int);
	void pauseButtonHandler();
};

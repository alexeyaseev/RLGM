#include "ControlPanel.h"
#include <QColorDialog>

ControlPanel::ControlPanel(RLGM::Interface* rlgm, 
						   std::map<std::pair<int,int>, ColorCorrection8bit>* color_corrections_8bit, 
						   std::map<std::pair<int, int>, ColorCorrection2bit>* color_corrections_2bit,
						   std::map<std::pair<int, int>, ColorCorrectionTails>* color_corrections_Tails,
						   std::map<int, UnificationCoef>* unification_coefs,
						   QApplication* owner, bool* pause_flag, QWidget *parent)
	: QWidget(parent)
{
	moduleRLGM = rlgm;
	ownerApp = owner;
	pause = pause_flag;
	colorCorrections8bit = color_corrections_8bit;
	colorCorrections2bit = color_corrections_2bit;
	colorCorrectionsTails = color_corrections_Tails;
	unificationCoefs = unification_coefs;

	ui.setupUi(this);
	
	this->move(1600, 0);
	connect(ui.pushButton_5, SIGNAL(clicked()), this, SLOT(buttonExitHandler()));
	ui.spinBox->setRange(0, moduleRLGM->views.size());
	ui.spinBox->setValue(1);
	ui.spinBox_2->setRange(0, moduleRLGM->radars.size());
	ui.spinBox_2->setValue(1);

	UpdateControls();

	connect(ui.spinBox, SIGNAL(valueChanged(int)), this, SLOT(viewChangedHandler(int)));
	connect(ui.spinBox_2, SIGNAL(valueChanged(int)), this, SLOT(radarChangedHandler(int)));
	connect(ui.horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(lowThr8bitChanged(int)));
	connect(ui.horizontalSlider_7, SIGNAL(valueChanged(int)), this, SLOT(highThr8bitChanged(int)));
	connect(ui.horizontalSlider_3, SIGNAL(valueChanged(int)), this, SLOT(gamma8bitChanged(int)));
	connect(ui.horizontalSlider_2, SIGNAL(valueChanged(int)), this, SLOT(hue8bitChanged(int)));
	connect(ui.horizontalSlider_4, SIGNAL(valueChanged(int)), this, SLOT(sat8bitChanged(int)));
	connect(ui.horizontalSlider_6, SIGNAL(valueChanged(int)), this, SLOT(startValueUnification8bitChanged(int)));
	connect(ui.horizontalSlider_5, SIGNAL(valueChanged(int)), this, SLOT(stopValueUnification8bitChanged(int)));
	connect(ui.checkBox, SIGNAL(clicked(bool)), this, SLOT(draw8bitFlagChanged(bool)));

	connect(ui.radioButton_3, SIGNAL(toggled(bool)), this, SLOT(textureSamplerTypeChanged(bool)));
	connect(ui.radioButton, SIGNAL(toggled(bool)), this, SLOT(unification8bitRuleChanged(bool)));

	connect(ui.horizontalSlider_20, SIGNAL(valueChanged(int)), this, SLOT(alphaThr8bitChanged(int)));

	connect(ui.checkBox_5, SIGNAL(clicked(bool)), this, SLOT(draw8bitLayerFlagChanged(bool)));
	connect(ui.checkBox_7, SIGNAL(clicked(bool)), this, SLOT(drawMapLayerFlagChanged(bool)));

	connect(ui.checkBox_4, SIGNAL(clicked(bool)), this, SLOT(draw2bitFlagChanged(bool)));

	connect(ui.checkBox_6, SIGNAL(clicked(bool)), this, SLOT(draw2bitLayerFlagChanged(bool)));
	connect(ui.checkBox_8, SIGNAL(clicked(bool)), this, SLOT(drawTailsLayerFlagChanged(bool)));

	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(button2bitColor1Handler()));
	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(button2bitColor2Handler()));
	connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(button2bitColor3Handler()));

	connect(ui.checkBox_3, SIGNAL(clicked(bool)), this, SLOT(drawTailsFlagChanged(bool)));

	connect(ui.horizontalSlider_15, SIGNAL(valueChanged(int)), this, SLOT(tailSizeChanged(int)));
	connect(ui.horizontalSlider_16, SIGNAL(valueChanged(int)), this, SLOT(tailMinLumaChanged(int)));
	connect(ui.horizontalSlider_18, SIGNAL(valueChanged(int)), this, SLOT(tailHueChanged(int)));
	connect(ui.horizontalSlider_19, SIGNAL(valueChanged(int)), this, SLOT(tailSatChanged(int)));

	connect(ui.pushButton, SIGNAL(clicked(bool)), this, SLOT(clearTailsHandler()));

	connect(ui.pushButton_6, SIGNAL(clicked(bool)), this, SLOT(pauseButtonHandler()));
}

ControlPanel::~ControlPanel()
{
}

void ControlPanel::buttonExitHandler()
{
	ownerApp->exit();
}

void ControlPanel::viewChangedHandler(int i) {
	UpdateControls();
}
void ControlPanel::radarChangedHandler(int i) {
	UpdateControls();
}
void ControlPanel::lowThr8bitChanged(int value) {
	int radarId = GetRadarId();
	int viewId = GetViewId();
	double dvalue = (double)value / ui.horizontalSlider->maximum();
	for (auto& view : moduleRLGM->views)
		if ((view.first == viewId) || (viewId == -1))
			for (auto& radar : moduleRLGM->radars)
				if ((radar.first == radarId) || (radarId == -1)) {
					std::pair<int, int> key = std::make_pair(view.first, radar.first);
					colorCorrections8bit->at(key).SetLowThr(dvalue);
					moduleRLGM->SetPRLIColorCorrection(view.first, radar.first, RLGM::PRLIType::PRLI_8bit, 256, colorCorrections8bit->at(key).colorsRGBA);
				}
}
void ControlPanel::highThr8bitChanged(int value) {
	int radarId = GetRadarId();
	int viewId = GetViewId();
	double dvalue = (double)value / ui.horizontalSlider_7->maximum();
	for (auto& view : moduleRLGM->views)
		if ((view.first == viewId) || (viewId == -1))
			for (auto& radar : moduleRLGM->radars)
				if ((radar.first == radarId) || (radarId == -1)) {
					std::pair<int, int> key = std::make_pair(view.first, radar.first);
					colorCorrections8bit->at(key).SetHighThr(dvalue);
					moduleRLGM->SetPRLIColorCorrection(view.first, radar.first, RLGM::PRLIType::PRLI_8bit, 256, colorCorrections8bit->at(key).colorsRGBA);
				}
}
void ControlPanel::gamma8bitChanged(int value) {
	int radarId = GetRadarId();
	int viewId = GetViewId();
	double dvalue = (double)value / ui.horizontalSlider_3->maximum();
	for (auto& view : moduleRLGM->views)
		if ((view.first == viewId) || (viewId == -1))
			for (auto& radar : moduleRLGM->radars)
				if ((radar.first == radarId) || (radarId == -1)) {
					std::pair<int, int> key = std::make_pair(view.first, radar.first);
					colorCorrections8bit->at(key).SetGamma(dvalue);
					moduleRLGM->SetPRLIColorCorrection(view.first, radar.first, RLGM::PRLIType::PRLI_8bit, 256, colorCorrections8bit->at(key).colorsRGBA);
				}
}
void ControlPanel::hue8bitChanged(int value) {
	int radarId = GetRadarId();
	int viewId = GetViewId();
	double dvalue = (double)value / ui.horizontalSlider_2->maximum();
	for (auto& view : moduleRLGM->views)
		if ((view.first == viewId) || (viewId == -1))
			for (auto& radar : moduleRLGM->radars)
				if ((radar.first == radarId) || (radarId == -1)) {
					std::pair<int, int> key = std::make_pair(view.first, radar.first);
					colorCorrections8bit->at(key).SetHue(dvalue);
					moduleRLGM->SetPRLIColorCorrection(view.first, radar.first, RLGM::PRLIType::PRLI_8bit, 256, colorCorrections8bit->at(key).colorsRGBA);
				}
}
void ControlPanel::sat8bitChanged(int value) {
	int radarId = GetRadarId();
	int viewId = GetViewId();
	double dvalue = (double)value / ui.horizontalSlider->maximum();
	for (auto& view : moduleRLGM->views)
		if ((view.first == viewId) || (viewId == -1))
			for (auto& radar : moduleRLGM->radars)
				if ((radar.first == radarId) || (radarId == -1)) {
					std::pair<int, int> key = std::make_pair(view.first, radar.first);
					colorCorrections8bit->at(key).SetSaturation(dvalue);
					moduleRLGM->SetPRLIColorCorrection(view.first, radar.first, RLGM::PRLIType::PRLI_8bit, 256, colorCorrections8bit->at(key).colorsRGBA);
				}
}
void ControlPanel::alphaThr8bitChanged(int value) {
	int radarId = GetRadarId();
	int viewId = GetViewId();
	double dvalue = (double)value / ui.horizontalSlider_20->maximum();
	for (auto& view : moduleRLGM->views)
		if ((view.first == viewId) || (viewId == -1))
			for (auto& radar : moduleRLGM->radars)
				if ((radar.first == radarId) || (radarId == -1)) {
					std::pair<int, int> key = std::make_pair(view.first, radar.first);
					colorCorrections8bit->at(key).SetAlphaThr(dvalue);
					moduleRLGM->SetPRLIColorCorrection(view.first, radar.first, RLGM::PRLIType::PRLI_8bit, 256, colorCorrections8bit->at(key).colorsRGBA);
				}
}

void ControlPanel::startValueUnification8bitChanged(int value) {
	int radarId = GetRadarId();
	double dvalue = (double)value / ui.horizontalSlider_6->maximum();
	for (auto& radar : moduleRLGM->radars)
		if ((radar.first == radarId) || (radarId == -1)) {
			unificationCoefs->at(radar.first).SetStartValue(dvalue);
			moduleRLGM->SetUnificationCoefs(radar.first, unificationCoefs->at(radar.first).m_coefs);
		}
}
void ControlPanel::stopValueUnification8bitChanged(int value) {
	int radarId = GetRadarId();
	double dvalue = (double)value / ui.horizontalSlider_5->maximum();
	for (auto& radar : moduleRLGM->radars)
		if ((radar.first == radarId) || (radarId == -1)) {
			unificationCoefs->at(radar.first).SetStopValue(dvalue);
			moduleRLGM->SetUnificationCoefs(radar.first, unificationCoefs->at(radar.first).m_coefs);
		}
}
void ControlPanel::draw8bitFlagChanged(bool checked) {
	moduleRLGM->SetShowPRLI(GetViewId(), GetRadarId(), RLGM::PRLIType::PRLI_8bit, checked);
}
void ControlPanel::draw2bitFlagChanged(bool checked) {
	moduleRLGM->SetShowPRLI(GetViewId(), GetRadarId(), RLGM::PRLIType::PRLI_2bit, checked);
}

void ControlPanel::drawTailsFlagChanged(bool checked) {
	moduleRLGM->SetShowPRLI(GetViewId(), GetRadarId(), RLGM::PRLIType::PRLI_Tail, checked);
}

void ControlPanel::textureSamplerTypeChanged(bool checked) {
	moduleRLGM->SetTextureSamplerType(GetViewId(), checked ? RLGM::TextureSamplerType::POINT : RLGM::TextureSamplerType::LINEAR);
}
void ControlPanel::unification8bitRuleChanged(bool checked) {
	moduleRLGM->Set8bitUnificationRule(GetViewId(), checked ? RLGM::PRLIUnificationRule::MAX : RLGM::PRLIUnificationRule::LINEAR);
}
void ControlPanel::draw8bitLayerFlagChanged(bool checked) {
	moduleRLGM->SetShow8bit(GetViewId(), checked);
}
void ControlPanel::draw2bitLayerFlagChanged(bool checked) {
	moduleRLGM->SetShow2bit(GetViewId(), checked);
}
void ControlPanel::drawTailsLayerFlagChanged(bool checked) {
	moduleRLGM->SetShowTails(GetViewId(), checked);
}


void ControlPanel::tailSizeChanged(int value) {
	int radarId = GetRadarId();
	int viewId = GetViewId();
	double dvalue = 1 - (double)value / ui.horizontalSlider_15->maximum();
	for (auto& view : moduleRLGM->views)
		if ((view.first == viewId) || (viewId == -1))
			for (auto& radar : moduleRLGM->radars)
				if ((radar.first == radarId) || (radarId == -1)) {
					std::pair<int, int> key = std::make_pair(view.first, radar.first);
					colorCorrectionsTails->at(key).SetLowThr(dvalue);
					moduleRLGM->SetPRLIColorCorrection(view.first, radar.first, RLGM::PRLIType::PRLI_Tail, 256, colorCorrectionsTails->at(key).colorsRGBA);
				}
}
void ControlPanel::tailMinLumaChanged(int value) {
	int radarId = GetRadarId();
	int viewId = GetViewId();
	double dvalue = (double)value / ui.horizontalSlider_16->maximum();
	for (auto& view : moduleRLGM->views)
		if ((view.first == viewId) || (viewId == -1))
			for (auto& radar : moduleRLGM->radars)
				if ((radar.first == radarId) || (radarId == -1)) {
					std::pair<int, int> key = std::make_pair(view.first, radar.first);
					colorCorrectionsTails->at(key).SetMinLuma(dvalue);
					moduleRLGM->SetPRLIColorCorrection(view.first, radar.first, RLGM::PRLIType::PRLI_Tail, 256, colorCorrectionsTails->at(key).colorsRGBA);
				}
}
void ControlPanel::tailHueChanged(int value) {
	int radarId = GetRadarId();
	int viewId = GetViewId();
	double dvalue = (double)value / ui.horizontalSlider_18->maximum();
	for (auto& view : moduleRLGM->views)
		if ((view.first == viewId) || (viewId == -1))
			for (auto& radar : moduleRLGM->radars)
				if ((radar.first == radarId) || (radarId == -1)) {
					std::pair<int, int> key = std::make_pair(view.first, radar.first);
					colorCorrectionsTails->at(key).SetHue(dvalue);
					moduleRLGM->SetPRLIColorCorrection(view.first, radar.first, RLGM::PRLIType::PRLI_Tail, 256, colorCorrectionsTails->at(key).colorsRGBA);
				}
}
void ControlPanel::tailSatChanged(int value) {
	int radarId = GetRadarId();
	int viewId = GetViewId();
	double dvalue = (double)value / ui.horizontalSlider_16->maximum();
	for (auto& view : moduleRLGM->views)
		if ((view.first == viewId) || (viewId == -1))
			for (auto& radar : moduleRLGM->radars)
				if ((radar.first == radarId) || (radarId == -1)) {
					std::pair<int, int> key = std::make_pair(view.first, radar.first);
					colorCorrectionsTails->at(key).SetSaturation(dvalue);
					moduleRLGM->SetPRLIColorCorrection(view.first, radar.first, RLGM::PRLIType::PRLI_Tail, 256, colorCorrectionsTails->at(key).colorsRGBA);
				}
}

void ControlPanel::drawMapLayerFlagChanged(bool checked) {
	/*if (checked) {
		moduleRLGM->SetShowPRLI(-1, 1, RLGM::PRLIType::PRLI_8bit, true);
		moduleRLGM->SetShowPRLI(-1, 2, RLGM::PRLIType::PRLI_8bit, false);
	}
	else {
		moduleRLGM->SetShowPRLI(-1, 1, RLGM::PRLIType::PRLI_8bit, false);
		moduleRLGM->SetShowPRLI(-1, 2, RLGM::PRLIType::PRLI_8bit, true);
	}*/
	moduleRLGM->SetShowMap(GetViewId(), checked);
}

void ControlPanel::button2bitColor1Handler() {
	QColor color = QColorDialog::getColor(Qt::yellow, this, "1", QColorDialog::ShowAlphaChannel);
	if (!color.isValid()) return;
	int temp = color.red();
	color.setRed(color.blue());
	color.setBlue(temp);
	int radarId = GetRadarId();
	int viewId = GetViewId();
	for (auto& view : moduleRLGM->views)
		if ((view.first == viewId) || (viewId == -1))
			for (auto& radar : moduleRLGM->radars)
				if ((radar.first == radarId) || (radarId == -1)) {
					std::pair<int, int> key = std::make_pair(view.first, radar.first);
					colorCorrections2bit->at(key).colorsRGBA[1] = color.rgba();
					moduleRLGM->SetPRLIColorCorrection(view.first, radar.first, RLGM::PRLIType::PRLI_2bit, 256, colorCorrections2bit->at(key).colorsRGBA);
				}
	UpdateControls();
}
void ControlPanel::button2bitColor2Handler() {
	QColor color = QColorDialog::getColor(Qt::yellow, this);
	if (!color.isValid()) return;
	int temp = color.red();
	color.setRed(color.blue());
	color.setBlue(temp);
	int radarId = GetRadarId();
	int viewId = GetViewId();
	for (auto& view : moduleRLGM->views)
		if ((view.first == viewId) || (viewId == -1))
			for (auto& radar : moduleRLGM->radars)
				if ((radar.first == radarId) || (radarId == -1)) {
					std::pair<int, int> key = std::make_pair(view.first, radar.first);
					colorCorrections2bit->at(key).colorsRGBA[2] = color.rgba();
					moduleRLGM->SetPRLIColorCorrection(view.first, radar.first, RLGM::PRLIType::PRLI_2bit, 256, colorCorrections2bit->at(key).colorsRGBA);
				}
	UpdateControls();
}
void ControlPanel::button2bitColor3Handler() {
	QColor color = QColorDialog::getColor(Qt::yellow, this);
	if (!color.isValid()) return;
	int temp = color.red();
	color.setRed(color.blue());
	color.setBlue(temp);
	int radarId = GetRadarId();
	int viewId = GetViewId();
	for (auto& view : moduleRLGM->views)
		if ((view.first == viewId) || (viewId == -1))
			for (auto& radar : moduleRLGM->radars)
				if ((radar.first == radarId) || (radarId == -1)) {
					std::pair<int, int> key = std::make_pair(view.first, radar.first);
					colorCorrections2bit->at(key).colorsRGBA[3] = color.rgba();
					moduleRLGM->SetPRLIColorCorrection(view.first, radar.first, RLGM::PRLIType::PRLI_2bit, 256, colorCorrections2bit->at(key).colorsRGBA);
				}
	UpdateControls();
}
void ControlPanel::clearTailsHandler() {
	moduleRLGM->ClearTailes(GetRadarId());
}

int ControlPanel::GetRadarId() {
	int result = ui.spinBox_2->value();
	if (result == 0)
		result = -1;
	return result;
}
int ControlPanel::GetViewId() {
	int result = ui.spinBox->value();
	if (result == 0)
		result = -1;
	return result;
}
void ControlPanel::UpdateControls()
{
	int radarId = GetRadarId();
	if (radarId == -1)
		radarId = 1;

	int viewId = GetViewId();
	if (viewId == -1)
		viewId = 1;

	std::pair<int, int> key = std::make_pair(viewId, radarId);
	
	//8 битка
	int value = (*colorCorrections8bit).at(key).GetLowThr() * ui.horizontalSlider->maximum();
	ui.horizontalSlider->setValue(value);
	value = (*colorCorrections8bit).at(key).GetHighThr() * ui.horizontalSlider_7->maximum();
	ui.horizontalSlider_7->setValue(value);
	value = (*colorCorrections8bit).at(key).GetGamma() * ui.horizontalSlider_3->maximum();
	ui.horizontalSlider_3->setValue(value);
	value = (*colorCorrections8bit).at(key).GetHue() * ui.horizontalSlider_2->maximum();
	ui.horizontalSlider_2->setValue(value);
	value = (*colorCorrections8bit).at(key).GetSaturation() * ui.horizontalSlider_4->maximum();
	ui.horizontalSlider_4->setValue(value);
	ui.checkBox->setChecked(moduleRLGM->GetShowPRLI(viewId, radarId, RLGM::PRLIType::PRLI_8bit));
	value = (*unificationCoefs).at(radarId).GetStartValue() * ui.horizontalSlider_6->maximum();
	ui.horizontalSlider_6->setValue(value);
	value = (*unificationCoefs).at(radarId).GetStopValue() * ui.horizontalSlider_5->maximum();
	ui.horizontalSlider_5->setValue(value);
	value = (*colorCorrections8bit).at(key).GetAlphaThr() * ui.horizontalSlider_20->maximum();
	ui.horizontalSlider_20->setValue(value);

	//текстурирование
	if (moduleRLGM->GetTextureSamplerType(viewId) == RLGM::TextureSamplerType::POINT)
		ui.radioButton_3->setChecked(true);
	else 
		ui.radioButton_4->setChecked(true);

	//режим склейки 8 биток
	if (moduleRLGM->Get8bitUnificationRule(viewId) == RLGM::PRLIUnificationRule::MAX)
		ui.radioButton->setChecked(true);
	else
		ui.radioButton_4->setChecked(true);

	//2 битка
	QColor color1 = QColor((*colorCorrections2bit).at(key).colorsRGBA[1]);
	int temp = color1.red();
	color1.setRed(color1.blue());
	color1.setBlue(temp);
	QString qss1 = QString("background-color: %1").arg(color1.name());
	QColor color2 = QColor((*colorCorrections2bit).at(key).colorsRGBA[2]);
	temp = color2.red();
	color2.setRed(color2.blue());
	color2.setBlue(temp);
	QString qss2 = QString("background-color: %1").arg(color2.name());
	QColor color3 = QColor((*colorCorrections2bit).at(key).colorsRGBA[3]);
	temp = color3.red();
	color3.setRed(color3.blue());
	color3.setBlue(temp);
	QString qss3 = QString("background-color: %1").arg(color3.name());
	ui.pushButton_2->setStyleSheet(qss1);
	ui.pushButton_3->setStyleSheet(qss2);
	ui.pushButton_4->setStyleSheet(qss3);
	ui.checkBox_4->setChecked(moduleRLGM->GetShowPRLI(viewId, radarId, RLGM::PRLIType::PRLI_2bit));

	//Хвосты
	value = (1 - (*colorCorrectionsTails).at(key).GetLowThr()) * ui.horizontalSlider_15->maximum();
	ui.horizontalSlider_15->setValue(value);
	value = (*colorCorrectionsTails).at(key).GetMinLuma() * ui.horizontalSlider_16->maximum();
	ui.horizontalSlider_16->setValue(value);
	value = (*colorCorrectionsTails).at(key).GetHue() * ui.horizontalSlider_18->maximum();
	ui.horizontalSlider_18->setValue(value);
	value = (*colorCorrectionsTails).at(key).GetSaturation() * ui.horizontalSlider_19->maximum();
	ui.horizontalSlider_19->setValue(value);
	ui.checkBox_3->setChecked(moduleRLGM->GetShowPRLI(viewId, radarId, RLGM::PRLIType::PRLI_Tail));

	//прозрачность + отображение слоёв
	ui.checkBox_5->setChecked(moduleRLGM->GetShow8bit(viewId));
	ui.checkBox_7->setChecked(moduleRLGM->GetShowMap(viewId));
	ui.checkBox_6->setChecked(moduleRLGM->GetShow2bit(viewId));
	ui.checkBox_8->setChecked(moduleRLGM->GetShowTails(viewId));
}

void ControlPanel::pauseButtonHandler()
{
	*pause = (*pause) ? false : true;
}


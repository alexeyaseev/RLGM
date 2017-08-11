#include "ViewForm.h"

ViewForm::ViewForm(bool loupe, RLGM::Interface* rlgm, int view_id, QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	moduleRLGM = rlgm;
	viewId = view_id;

	QDesktopWidget dw;
	QRect mainScreenSize = dw.availableGeometry(dw.primaryScreen());
	int w = mainScreenSize.width();
	int h = mainScreenSize.height();
	if (loupe) //fixed percent of screen size
	{
		w *= 0.4f;
		h *= 0.4f;
		this->setWindowFlags(Qt::WindowStaysOnTopHint);
	}
	else
	{
		this->setWindowFlags(Qt::FramelessWindowHint);
	}
	this->setFixedSize(w, h);	
}

void ViewForm::wheelEvent(QWheelEvent *ev)
{
	int offset_x = ev->pos().x() - this->size().width()  / 2;
	int offset_y = ev->pos().y() - this->size().height() / 2;

	float scale = 1.3f;

	if (ev->angleDelta().y() > 0)
	{
		moduleRLGM->MultiplyDPPCenter(viewId, -1, scale);
		moduleRLGM->AddRadarPos(viewId, -1, -(int)(offset_x * (scale - 1)), -(int)(offset_y * (scale - 1)));

		moduleRLGM->MultiplySurfacesDPPCenter(viewId, scale);
		moduleRLGM->AddSurfacesShift(viewId, -(int)(offset_x * (scale - 1)), -(int)(offset_y * (scale - 1)));
	}
	else
	{
		moduleRLGM->MultiplyDPPCenter(viewId, -1, 1 / scale);
		moduleRLGM->AddRadarPos(viewId, -1, +(int)(offset_x * (1 - 1 / scale)), +(int)(offset_y * (1 - 1 / scale)));

		moduleRLGM->MultiplySurfacesDPPCenter(viewId, 1 / scale);
		moduleRLGM->AddSurfacesShift(viewId, +(int)(offset_x * (1 - 1 / scale)), +(int)(offset_y * (1 - 1 / scale)));
	}

}

void ViewForm::closeEvent(QCloseEvent *ev)
{
	moduleRLGM->DeleteView(viewId);
}

void ViewForm::mouseMoveEvent(QMouseEvent *ev)
{
	if (ev->buttons() == Qt::LeftButton)
	{
		moduleRLGM->AddRadarPos(viewId, -1, ev->pos().x() - mousePosX, ev->pos().y() - mousePosY);
		moduleRLGM->AddSurfacesShift(viewId, ev->pos().x() - mousePosX, ev->pos().y() - mousePosY);
		mousePosX = ev->pos().x();
		mousePosY = ev->pos().y();
	}
}

void ViewForm::mousePressEvent(QMouseEvent *ev)
{
	mousePosX = ev->pos().x();
	mousePosY = ev->pos().y();
}
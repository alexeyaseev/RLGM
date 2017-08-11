#pragma once

#include <QtWidgets/QMainWindow>
#include <QDesktopWidget>
#include <QCloseEvent>
#include "ui_ViewForm.h"

#include "../../RLGM/Interface.h"

class ViewForm : public QMainWindow
{
	//Q_OBJECT

	RLGM::Interface* moduleRLGM = nullptr;
	int viewId;

	int mousePosX, mousePosY;

public:
	ViewForm(bool loupe, RLGM::Interface* rlgm, int view_id, QWidget *parent = Q_NULLPTR);

private:
	Ui::QtGuiApplication1Class ui;

	void wheelEvent(QWheelEvent *ev);
	void closeEvent(QCloseEvent *ev);
	void mouseMoveEvent(QMouseEvent *ev);
	void mousePressEvent(QMouseEvent *ev);
};

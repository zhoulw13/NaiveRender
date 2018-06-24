#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_NaiveRender.h"
#include "CloseGL.h"

#include <qlabel.h>
#include <qimage.h>

class NaiveRender : public QMainWindow
{
	Q_OBJECT

public:
	NaiveRender(QWidget *parent = Q_NULLPTR);
	void InitUI();
	void ReRender();

public slots:
	void LoadObj();

private:
	Ui::NaiveRenderClass ui;
	CloseGL *backend;

	QLabel *displayLabel;
};

#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_NaiveRender.h"
#include "CloseGL.h"

class NaiveRender : public QMainWindow
{
	Q_OBJECT

public:
	NaiveRender(QWidget *parent = Q_NULLPTR);
	void InitUI();

public slots:
	void LoadObj();

private:
	Ui::NaiveRenderClass ui;
	CloseGL *backend;
};

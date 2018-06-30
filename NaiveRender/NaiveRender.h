#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_NaiveRender.h"
#include "CloseGL.h"

#include <qlabel.h>
#include <qimage.h>
#include <QKeyEvent>

class NaiveRender : public QMainWindow
{
	Q_OBJECT

public:
	NaiveRender(QWidget *parent = Q_NULLPTR);
	void InitUI();
	void ReRender();

protected:
	void keyPressEvent(QKeyEvent *);

public slots:
	void LoadObj();
	void LoadTexture();
	void ResetView();
	void ColorScheme();
	void Projection();
	void SetRayTracing();

private:
	Ui::NaiveRenderClass ui;
	CloseGL *backend;
	bool loaded;

	QLabel *displayLabel;

	QAction *segmentAct;
	QAction *faceAct;
	QActionGroup *colorSchemeGroup;

	QAction *persAct;
	QAction *orthAct;
	QActionGroup *projectionGroup;
};

#include "NaiveRender.h"
#include <iostream>
using namespace std;

#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qfiledialog.h>

NaiveRender::NaiveRender(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	InitUI();

	backend = new CloseGL();
}

void NaiveRender::InitUI() {
	QToolBar *toolBar = addToolBar("Tool");

	QToolButton *loadBtn = new QToolButton;
	QPixmap load("Resources/open-folder-full.png");
	loadBtn->setIcon(QIcon(load));
	loadBtn->setToolTip("Load");
	connect(loadBtn, SIGNAL(clicked(bool)), this, SLOT(LoadObj()));
	toolBar->addWidget(loadBtn);
	toolBar->addSeparator();
}

void NaiveRender::LoadObj() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Load Obj File"),
		".", tr("Obj Files (*.obj)"));

	if (!fileName.isEmpty()) {
		fileName.replace('/', '\\');
		string file = fileName.toLocal8Bit().constData();
		backend->readfile(file.c_str());
	}
}
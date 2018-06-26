#include "NaiveRender.h"
#include "constants.h"
#include <iostream>
using namespace std;

#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qfiledialog.h>
#include <qpixmap.h>

NaiveRender::NaiveRender(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	InitUI();

	backend = new CloseGL();
	loaded = false;
	displayLabel->resize(QSize(window_width, window_height));
	ReRender();
}

void NaiveRender::InitUI() {
	displayLabel = new QLabel(this);
	setCentralWidget(displayLabel);

	QToolBar *toolBar = addToolBar("Tool");

	QToolButton *loadBtn = new QToolButton;
	QPixmap load("Resources/obj.png");
	loadBtn->setIcon(QIcon(load));
	loadBtn->setToolTip("Load");
	connect(loadBtn, SIGNAL(clicked(bool)), this, SLOT(LoadObj()));
	toolBar->addWidget(loadBtn);
	toolBar->addSeparator();

	QToolButton *resetBtn = new QToolButton;
	QPixmap reset("Resources/reset.png");
	resetBtn->setIcon(QIcon(reset));
	resetBtn->setToolTip("Reset View");
	connect(resetBtn, SIGNAL(clicked(bool)), this, SLOT(ResetView()));
	toolBar->addWidget(resetBtn);
}

void NaiveRender::ReRender() {
	backend->render();
	QImage img = QImage(backend->get_data(), window_width, window_height, QImage::Format_RGB888);
	displayLabel->setPixmap(QPixmap::fromImage(img));
}

void NaiveRender::LoadObj() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Load Obj File"),
		".", tr("Obj Files (*.obj)"));

	if (!fileName.isEmpty()) {
		fileName.replace('/', '\\');
		string file = fileName.toLocal8Bit().constData();
		backend->readfile(file.c_str());
		loaded = true;
		ReRender();
	}
}

void NaiveRender::ResetView() {

}

void NaiveRender::keyPressEvent(QKeyEvent *event) {
	if (!loaded)
		return;
	cout << event->key() << endl;
	if (event->key() == Qt::Key::Key_W)
		backend->camera_move(TRANSITION_UP);
	else if (event->key() == Qt::Key::Key_S)
		backend->camera_move(TRANSITION_DOWN);
	else if (event->key() == Qt::Key::Key_A)
		backend->camera_move(TRANSITION_LEFT);
	else if (event->key() == Qt::Key::Key_D)
		backend->camera_move(TRANSITION_RIGHT);
	else if (event->key() == Qt::Key::Key_I)
		backend->camera_move(ROTATE_UP);
	else if (event->key() == Qt::Key::Key_K)
		backend->camera_move(ROTATE_DOWN);
	else if (event->key() == Qt::Key::Key_J)
		backend->camera_move(ROTATE_LEFT);
	else if (event->key() == Qt::Key::Key_L)
		backend->camera_move(ROTATE_RIGHT);
	else
		return;
	ReRender();
}
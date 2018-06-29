#include "NaiveRender.h"
#include "constants.h"
#include <iostream>
using namespace std;

#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qfiledialog.h>
#include <qpixmap.h>
#include <qbuttongroup.h>
#include <qbuffer.h>

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
	
	QMenuBar *mb = menuBar();
	QMenu *file = mb->addMenu("&File");
	file->addAction("&Load Obj", this, SLOT(LoadObj()));
	file->addAction("&Load Texture", this, SLOT(LoadTexture()));
	file->addSeparator();
	file->addAction("&Quit", qApp, SLOT(quit()));

	QMenu *view = mb->addMenu("&View");
	view->addAction("&Reset View", this, SLOT(ResetView()));
	view->addSeparator();
	QMenu *colorSchemeMenu = view->addMenu("&Color Scheme");
	view->addSeparator();
	QMenu *projectionMenu = view->addMenu("&Projection");

	//color scheme menu group
	colorSchemeGroup = new QActionGroup(this);

	segmentAct = new QAction(tr("&Segment"), colorSchemeGroup);
	segmentAct->setCheckable(true);
	segmentAct->setStatusTip(tr("Colorize Segment"));
	connect(segmentAct, &QAction::triggered, this, &NaiveRender::ColorScheme);

	faceAct = new QAction(tr("&Face"), colorSchemeGroup);
	faceAct->setCheckable(true);
	faceAct->setStatusTip(tr("Colorize Face"));
	connect(faceAct, &QAction::triggered, this, &NaiveRender::ColorScheme);

	segmentAct->setChecked(true);
	colorSchemeGroup->setExclusive(true);

	colorSchemeMenu->addActions(colorSchemeGroup->actions());

	//projection menu group
	projectionGroup = new QActionGroup(this);

	persAct = new QAction(tr("&Perspective"), projectionGroup);
	persAct->setCheckable(true);
	persAct->setStatusTip(tr("Perspective Projection"));
	connect(persAct, &QAction::triggered, this, &NaiveRender::Projection);
	
	orthAct = new QAction(tr("&Orthogonal"), projectionGroup);
	orthAct->setCheckable(true);
	orthAct->setStatusTip(tr("Orthogonal Projection"));
	connect(orthAct, &QAction::triggered, this, &NaiveRender::Projection);

	persAct->setChecked(true);
	projectionGroup->setExclusive(true);

	projectionMenu->addActions(projectionGroup->actions());

	mb->show();
	setMenuBar(mb);
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
		ResetView();
	}
}

void NaiveRender::LoadTexture() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Load Texture File"),
		".", tr("Image Files (*.png)"));

	if (!fileName.isEmpty()) {
		fileName.replace('/', '\\');
		string file = fileName.toLocal8Bit().constData();
		QImage img(fileName);
		img = img.convertToFormat(QImage::Format_RGB888);
		QByteArray bytes;
		QBuffer buffer(&bytes);
		buffer.open(QIODevice::WriteOnly);
		img.save(&buffer, "PNG");
		QColor color(img.pixel(0, 0));
		cout << color.red() << endl;

		unsigned char * t8 = (unsigned char *)img.data_ptr();
		cout << t8[0] << " " << t8[1] << " " << t8[2] << endl;

		QFile* imageFile = new QFile(fileName);
		imageFile->open(QIODevice::ReadOnly);
		QByteArray bytes2 = imageFile->readAll();
		cout << unsigned(bytes2[0]);
		bytes2.data();

		int ind = 0; 28 * 512 + 13;
		char * texture = bytes.data();
		cout << unsigned(texture[ind + 0]) << " " << unsigned(texture[ind + 1]) << " " << unsigned(texture[ind + 2]) << endl;

		unsigned char * t2 = (unsigned char*)bytes2.data();
		cout << unsigned(t2[ind + 0]) << " " << unsigned(t2[ind + 1]) << " " << unsigned(t2[ind + 2]) << endl;

		char *t3 = bytes2.data();
		cout << t3[0] << " " << t3[1] << " " << t3[2] << endl;

		const std::size_t count = bytes2.size();
		cout << count << endl;
		unsigned char* hex = new unsigned char[count];
		std::memcpy(hex, bytes2.constData(), count);
		cout << unsigned(hex[0]) << " " << unsigned(hex[1]) << " " << unsigned(hex[2]) << endl;

		

		backend->loadtexture((unsigned char*)bytes.data());
		ReRender();
	}
}

void NaiveRender::ResetView() {
	backend->reset_transform();
	ReRender();
}

void NaiveRender::ColorScheme() {
	if (colorSchemeGroup->checkedAction() == faceAct)
		backend->set_color_scheme(FACE);
	else if (colorSchemeGroup->checkedAction() == segmentAct)
		backend->set_color_scheme(SEGMENT);
	ReRender();
}

void NaiveRender::Projection() {
	if (projectionGroup->checkedAction() == persAct)
		backend->set_projection(PERSPECTIVE);
	else if (projectionGroup->checkedAction() == orthAct)
		backend->set_projection(ORTHOGONAL);
	ReRender();
}

void NaiveRender::keyPressEvent(QKeyEvent *event) {
	if (!loaded)
		return;
	cout << event->key() << endl;
	if (event->key() == Qt::Key::Key_W)
		backend->camera_move(TRANSLATE_UP);
	else if (event->key() == Qt::Key::Key_S)
		backend->camera_move(TRANSLATE_DOWN);
	else if (event->key() == Qt::Key::Key_A)
		backend->camera_move(TRANSLATE_LEFT);
	else if (event->key() == Qt::Key::Key_D)
		backend->camera_move(TRANSLATE_RIGHT);
	else if (event->key() == Qt::Key::Key_Q)
		backend->camera_move(TRANSLATE_IN);
	else if (event->key() == Qt::Key::Key_E)
		backend->camera_move(TRANSLATE_OUT);
	else if (event->key() == Qt::Key::Key_I)
		backend->camera_move(ROTATE_UP);
	else if (event->key() == Qt::Key::Key_K)
		backend->camera_move(ROTATE_DOWN);
	else if (event->key() == Qt::Key::Key_J)
		backend->camera_move(ROTATE_LEFT);
	else if (event->key() == Qt::Key::Key_L)
		backend->camera_move(ROTATE_RIGHT);
	else if (event->key() == Qt::Key::Key_Plus)
		backend->camera_move(SCALE_OUT);
	else if (event->key() == Qt::Key::Key_Minus)
		backend->camera_move(SCALE_IN);
	else
		return;
	ReRender();
}
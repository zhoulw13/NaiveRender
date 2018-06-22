/********************************************************************************
** Form generated from reading UI file 'NaiveRender.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NAIVERENDER_H
#define UI_NAIVERENDER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NaiveRenderClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *NaiveRenderClass)
    {
        if (NaiveRenderClass->objectName().isEmpty())
            NaiveRenderClass->setObjectName(QStringLiteral("NaiveRenderClass"));
        NaiveRenderClass->resize(600, 400);
        menuBar = new QMenuBar(NaiveRenderClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        NaiveRenderClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(NaiveRenderClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        NaiveRenderClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(NaiveRenderClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        NaiveRenderClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(NaiveRenderClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        NaiveRenderClass->setStatusBar(statusBar);

        retranslateUi(NaiveRenderClass);

        QMetaObject::connectSlotsByName(NaiveRenderClass);
    } // setupUi

    void retranslateUi(QMainWindow *NaiveRenderClass)
    {
        NaiveRenderClass->setWindowTitle(QApplication::translate("NaiveRenderClass", "NaiveRender", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class NaiveRenderClass: public Ui_NaiveRenderClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NAIVERENDER_H

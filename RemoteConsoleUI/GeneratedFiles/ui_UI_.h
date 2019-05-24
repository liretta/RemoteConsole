/********************************************************************************
** Form generated from reading UI file 'UI_.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UI__H
#define UI_UI__H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UI_Class
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *UI_Class)
    {
        if (UI_Class->objectName().isEmpty())
            UI_Class->setObjectName(QStringLiteral("UI_Class"));
        UI_Class->resize(600, 400);
        menuBar = new QMenuBar(UI_Class);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        UI_Class->setMenuBar(menuBar);
        mainToolBar = new QToolBar(UI_Class);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        UI_Class->addToolBar(mainToolBar);
        centralWidget = new QWidget(UI_Class);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        UI_Class->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(UI_Class);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        UI_Class->setStatusBar(statusBar);

        retranslateUi(UI_Class);

        QMetaObject::connectSlotsByName(UI_Class);
    } // setupUi

    void retranslateUi(QMainWindow *UI_Class)
    {
        UI_Class->setWindowTitle(QApplication::translate("UI_Class", "UI_", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UI_Class: public Ui_UI_Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UI__H

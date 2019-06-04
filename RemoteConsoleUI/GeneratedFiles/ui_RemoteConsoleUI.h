/********************************************************************************
** Form generated from reading UI file 'RemoteConsoleUI.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REMOTECONSOLEUI_H
#define UI_REMOTECONSOLEUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RemoteConsoleUIClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *RemoteConsoleUIClass)
    {
        if (RemoteConsoleUIClass->objectName().isEmpty())
            RemoteConsoleUIClass->setObjectName(QString::fromUtf8("RemoteConsoleUIClass"));
        RemoteConsoleUIClass->resize(600, 400);
        menuBar = new QMenuBar(RemoteConsoleUIClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        RemoteConsoleUIClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(RemoteConsoleUIClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        RemoteConsoleUIClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(RemoteConsoleUIClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        RemoteConsoleUIClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(RemoteConsoleUIClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        RemoteConsoleUIClass->setStatusBar(statusBar);

        retranslateUi(RemoteConsoleUIClass);

        QMetaObject::connectSlotsByName(RemoteConsoleUIClass);
    } // setupUi

    void retranslateUi(QMainWindow *RemoteConsoleUIClass)
    {
        RemoteConsoleUIClass->setWindowTitle(QApplication::translate("RemoteConsoleUIClass", "RemoteConsoleUI", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RemoteConsoleUIClass: public Ui_RemoteConsoleUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REMOTECONSOLEUI_H

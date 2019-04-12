/********************************************************************************
** Form generated from reading UI file 'ChessClient.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHESSCLIENT_H
#define UI_CHESSCLIENT_H

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

class Ui_ChessClientClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ChessClientClass)
    {
        if (ChessClientClass->objectName().isEmpty())
            ChessClientClass->setObjectName(QStringLiteral("ChessClientClass"));
        ChessClientClass->resize(600, 400);
        menuBar = new QMenuBar(ChessClientClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        ChessClientClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ChessClientClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ChessClientClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(ChessClientClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        ChessClientClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(ChessClientClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ChessClientClass->setStatusBar(statusBar);

        retranslateUi(ChessClientClass);

        QMetaObject::connectSlotsByName(ChessClientClass);
    } // setupUi

    void retranslateUi(QMainWindow *ChessClientClass)
    {
        ChessClientClass->setWindowTitle(QApplication::translate("ChessClientClass", "ChessClient", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChessClientClass: public Ui_ChessClientClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHESSCLIENT_H

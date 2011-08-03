/********************************************************************************
** Form generated from reading ui file 'traveling_main_window.ui'
**
** Created: Sun Jan 3 12:13:36 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef TRAVELING_MAIN_WINDOW_H
#define TRAVELING_MAIN_WINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TravelingMainWindowUI
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QFrame *_content_frame;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *TravelingMainWindowUI)
    {
        if (TravelingMainWindowUI->objectName().isEmpty())
            TravelingMainWindowUI->setObjectName(QString::fromUtf8("TravelingMainWindowUI"));
        TravelingMainWindowUI->resize(563, 430);
        centralwidget = new QWidget(TravelingMainWindowUI);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout->setMargin(9);
#endif
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        _content_frame = new QFrame(centralwidget);
        _content_frame->setObjectName(QString::fromUtf8("_content_frame"));
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(7), static_cast<QSizePolicy::Policy>(7));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(_content_frame->sizePolicy().hasHeightForWidth());
        _content_frame->setSizePolicy(sizePolicy);
        _content_frame->setFrameShape(QFrame::NoFrame);
        _content_frame->setFrameShadow(QFrame::Plain);

        gridLayout->addWidget(_content_frame, 0, 0, 1, 1);

        TravelingMainWindowUI->setCentralWidget(centralwidget);
        menubar = new QMenuBar(TravelingMainWindowUI);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 563, 30));
        TravelingMainWindowUI->setMenuBar(menubar);
        statusbar = new QStatusBar(TravelingMainWindowUI);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        TravelingMainWindowUI->setStatusBar(statusbar);

        retranslateUi(TravelingMainWindowUI);

        QMetaObject::connectSlotsByName(TravelingMainWindowUI);
    } // setupUi

    void retranslateUi(QMainWindow *TravelingMainWindowUI)
    {
        TravelingMainWindowUI->setWindowTitle(QApplication::translate("TravelingMainWindowUI", "MainWindow", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(TravelingMainWindowUI);
    } // retranslateUi

};

namespace Ui {
    class TravelingMainWindowUI: public Ui_TravelingMainWindowUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // TRAVELING_MAIN_WINDOW_H

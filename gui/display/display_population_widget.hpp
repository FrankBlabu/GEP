/********************************************************************************
** Form generated from reading ui file 'display_population_widget.ui'
**
** Created: Sun Jan 3 12:13:36 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef DISPLAY_POPULATION_WIDGET_H
#define DISPLAY_POPULATION_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QRadioButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DisplayPopulationWidgetUI
{
public:
    QGridLayout *gridLayout;
    QRadioButton *_show_selected;
    QRadioButton *_show_fittest;
    QRadioButton *_show_all;
    QFrame *_display;

    void setupUi(QWidget *DisplayPopulationWidgetUI)
    {
        if (DisplayPopulationWidgetUI->objectName().isEmpty())
            DisplayPopulationWidgetUI->setObjectName(QString::fromUtf8("DisplayPopulationWidgetUI"));
        DisplayPopulationWidgetUI->resize(578, 400);
        gridLayout = new QGridLayout(DisplayPopulationWidgetUI);
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
        gridLayout->setMargin(0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        _show_selected = new QRadioButton(DisplayPopulationWidgetUI);
        _show_selected->setObjectName(QString::fromUtf8("_show_selected"));

        gridLayout->addWidget(_show_selected, 3, 0, 1, 1);

        _show_fittest = new QRadioButton(DisplayPopulationWidgetUI);
        _show_fittest->setObjectName(QString::fromUtf8("_show_fittest"));

        gridLayout->addWidget(_show_fittest, 2, 0, 1, 1);

        _show_all = new QRadioButton(DisplayPopulationWidgetUI);
        _show_all->setObjectName(QString::fromUtf8("_show_all"));
        _show_all->setChecked(true);

        gridLayout->addWidget(_show_all, 1, 0, 1, 1);

        _display = new QFrame(DisplayPopulationWidgetUI);
        _display->setObjectName(QString::fromUtf8("_display"));
        _display->setFrameShape(QFrame::StyledPanel);
        _display->setFrameShadow(QFrame::Raised);

        gridLayout->addWidget(_display, 0, 0, 1, 1);


        retranslateUi(DisplayPopulationWidgetUI);

        QMetaObject::connectSlotsByName(DisplayPopulationWidgetUI);
    } // setupUi

    void retranslateUi(QWidget *DisplayPopulationWidgetUI)
    {
        DisplayPopulationWidgetUI->setWindowTitle(QApplication::translate("DisplayPopulationWidgetUI", "DisplayPopulationWidgetUI", 0, QApplication::UnicodeUTF8));
        _show_selected->setText(QApplication::translate("DisplayPopulationWidgetUI", "Show selected", 0, QApplication::UnicodeUTF8));
        _show_fittest->setText(QApplication::translate("DisplayPopulationWidgetUI", "Show fittest", 0, QApplication::UnicodeUTF8));
        _show_all->setText(QApplication::translate("DisplayPopulationWidgetUI", "Show all", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(DisplayPopulationWidgetUI);
    } // retranslateUi

};

namespace Ui {
    class DisplayPopulationWidgetUI: public Ui_DisplayPopulationWidgetUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // DISPLAY_POPULATION_WIDGET_H

/********************************************************************************
** Form generated from reading ui file 'display_status_widget.ui'
**
** Created: Sun Jan 3 12:13:36 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef DISPLAY_STATUS_WIDGET_H
#define DISPLAY_STATUS_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DisplayStatusWidgetUI
{
public:
    QGridLayout *gridLayout;
    QLineEdit *_max_fitness;
    QLabel *_max_fitness_label;
    QLineEdit *_average_fitness;
    QLabel *_average_fitness_label;
    QLabel *_step_label;
    QLineEdit *_step;

    void setupUi(QWidget *DisplayStatusWidgetUI)
    {
        if (DisplayStatusWidgetUI->objectName().isEmpty())
            DisplayStatusWidgetUI->setObjectName(QString::fromUtf8("DisplayStatusWidgetUI"));
        DisplayStatusWidgetUI->resize(367, 81);
        gridLayout = new QGridLayout(DisplayStatusWidgetUI);
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
        gridLayout->setMargin(0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        _max_fitness = new QLineEdit(DisplayStatusWidgetUI);
        _max_fitness->setObjectName(QString::fromUtf8("_max_fitness"));
        _max_fitness->setReadOnly(true);

        gridLayout->addWidget(_max_fitness, 2, 1, 1, 1);

        _max_fitness_label = new QLabel(DisplayStatusWidgetUI);
        _max_fitness_label->setObjectName(QString::fromUtf8("_max_fitness_label"));

        gridLayout->addWidget(_max_fitness_label, 2, 0, 1, 1);

        _average_fitness = new QLineEdit(DisplayStatusWidgetUI);
        _average_fitness->setObjectName(QString::fromUtf8("_average_fitness"));
        _average_fitness->setReadOnly(true);

        gridLayout->addWidget(_average_fitness, 1, 1, 1, 1);

        _average_fitness_label = new QLabel(DisplayStatusWidgetUI);
        _average_fitness_label->setObjectName(QString::fromUtf8("_average_fitness_label"));

        gridLayout->addWidget(_average_fitness_label, 1, 0, 1, 1);

        _step_label = new QLabel(DisplayStatusWidgetUI);
        _step_label->setObjectName(QString::fromUtf8("_step_label"));

        gridLayout->addWidget(_step_label, 0, 0, 1, 1);

        _step = new QLineEdit(DisplayStatusWidgetUI);
        _step->setObjectName(QString::fromUtf8("_step"));
        _step->setReadOnly(true);

        gridLayout->addWidget(_step, 0, 1, 1, 1);


        retranslateUi(DisplayStatusWidgetUI);

        QMetaObject::connectSlotsByName(DisplayStatusWidgetUI);
    } // setupUi

    void retranslateUi(QWidget *DisplayStatusWidgetUI)
    {
        DisplayStatusWidgetUI->setWindowTitle(QApplication::translate("DisplayStatusWidgetUI", "Form", 0, QApplication::UnicodeUTF8));
        _max_fitness_label->setText(QApplication::translate("DisplayStatusWidgetUI", "Maximum fitness", 0, QApplication::UnicodeUTF8));
        _average_fitness_label->setText(QApplication::translate("DisplayStatusWidgetUI", "Average fitness", 0, QApplication::UnicodeUTF8));
        _step_label->setText(QApplication::translate("DisplayStatusWidgetUI", "Step", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(DisplayStatusWidgetUI);
    } // retranslateUi

};

namespace Ui {
    class DisplayStatusWidgetUI: public Ui_DisplayStatusWidgetUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // DISPLAY_STATUS_WIDGET_H

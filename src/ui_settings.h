/********************************************************************************
** Form generated from reading UI file 'settings.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGS_H
#define UI_SETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Settings
{
public:
    QVBoxLayout *verticalLayout_2;
    QFormLayout *formLayout;
    QLabel *label_6;
    QComboBox *comboBoxAudioMode;
    QLabel *labelFrequency;
    QSlider *sliderFrequency;
    QLabel *label;
    QComboBox *comboBoxAudioCodec;
    QLabel *label_2;
    QComboBox *comboBoxSampleRate;
    QLabel *label_3;
    QComboBox *comboBoxSampleSize;
    QLabel *label_4;
    QComboBox *comboBoxSampleType;
    QLabel *label_5;
    QComboBox *comboBoxWindowFunction;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonQuit;

    void setupUi(QDialog *Settings)
    {
        if (Settings->objectName().isEmpty())
            Settings->setObjectName(QStringLiteral("Settings"));
        Settings->resize(424, 294);
        verticalLayout_2 = new QVBoxLayout(Settings);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        label_6 = new QLabel(Settings);
        label_6->setObjectName(QStringLiteral("label_6"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_6);

        comboBoxAudioMode = new QComboBox(Settings);
        comboBoxAudioMode->setObjectName(QStringLiteral("comboBoxAudioMode"));

        formLayout->setWidget(1, QFormLayout::FieldRole, comboBoxAudioMode);

        labelFrequency = new QLabel(Settings);
        labelFrequency->setObjectName(QStringLiteral("labelFrequency"));

        formLayout->setWidget(2, QFormLayout::LabelRole, labelFrequency);

        sliderFrequency = new QSlider(Settings);
        sliderFrequency->setObjectName(QStringLiteral("sliderFrequency"));
        sliderFrequency->setEnabled(false);
        sliderFrequency->setMinimum(20);
        sliderFrequency->setMaximum(4000);
        sliderFrequency->setSingleStep(20);
        sliderFrequency->setValue(440);
        sliderFrequency->setOrientation(Qt::Horizontal);

        formLayout->setWidget(2, QFormLayout::FieldRole, sliderFrequency);

        label = new QLabel(Settings);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label);

        comboBoxAudioCodec = new QComboBox(Settings);
        comboBoxAudioCodec->setObjectName(QStringLiteral("comboBoxAudioCodec"));

        formLayout->setWidget(3, QFormLayout::FieldRole, comboBoxAudioCodec);

        label_2 = new QLabel(Settings);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_2);

        comboBoxSampleRate = new QComboBox(Settings);
        comboBoxSampleRate->setObjectName(QStringLiteral("comboBoxSampleRate"));

        formLayout->setWidget(4, QFormLayout::FieldRole, comboBoxSampleRate);

        label_3 = new QLabel(Settings);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout->setWidget(5, QFormLayout::LabelRole, label_3);

        comboBoxSampleSize = new QComboBox(Settings);
        comboBoxSampleSize->setObjectName(QStringLiteral("comboBoxSampleSize"));

        formLayout->setWidget(5, QFormLayout::FieldRole, comboBoxSampleSize);

        label_4 = new QLabel(Settings);
        label_4->setObjectName(QStringLiteral("label_4"));

        formLayout->setWidget(6, QFormLayout::LabelRole, label_4);

        comboBoxSampleType = new QComboBox(Settings);
        comboBoxSampleType->setObjectName(QStringLiteral("comboBoxSampleType"));

        formLayout->setWidget(6, QFormLayout::FieldRole, comboBoxSampleType);

        label_5 = new QLabel(Settings);
        label_5->setObjectName(QStringLiteral("label_5"));

        formLayout->setWidget(7, QFormLayout::LabelRole, label_5);

        comboBoxWindowFunction = new QComboBox(Settings);
        comboBoxWindowFunction->setObjectName(QStringLiteral("comboBoxWindowFunction"));

        formLayout->setWidget(7, QFormLayout::FieldRole, comboBoxWindowFunction);


        verticalLayout_2->addLayout(formLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonQuit = new QPushButton(Settings);
        buttonQuit->setObjectName(QStringLiteral("buttonQuit"));
        buttonQuit->setCursor(QCursor(Qt::PointingHandCursor));
        buttonQuit->setAutoDefault(false);

        horizontalLayout->addWidget(buttonQuit);


        verticalLayout_2->addLayout(horizontalLayout);


        retranslateUi(Settings);
        QObject::connect(buttonQuit, SIGNAL(clicked()), Settings, SLOT(close()));

        comboBoxSampleRate->setCurrentIndex(-1);
        comboBoxSampleSize->setCurrentIndex(-1);
        comboBoxWindowFunction->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Settings);
    } // setupUi

    void retranslateUi(QDialog *Settings)
    {
        Settings->setWindowTitle(QApplication::translate("Settings", "Dialog", Q_NULLPTR));
        label_6->setText(QApplication::translate("Settings", "Audio Mode", Q_NULLPTR));
        comboBoxAudioMode->clear();
        comboBoxAudioMode->insertItems(0, QStringList()
         << QApplication::translate("Settings", "Microphone", Q_NULLPTR)
         << QApplication::translate("Settings", "Pre-Recorded", Q_NULLPTR)
         << QApplication::translate("Settings", "Generated", Q_NULLPTR)
        );
        labelFrequency->setText(QApplication::translate("Settings", "Generated Frequency (440  Hz)", Q_NULLPTR));
        label->setText(QApplication::translate("Settings", "Codec", Q_NULLPTR));
        label_2->setText(QApplication::translate("Settings", "Sample Rate", Q_NULLPTR));
        label_3->setText(QApplication::translate("Settings", "Sample Size", Q_NULLPTR));
        label_4->setText(QApplication::translate("Settings", "sample Type", Q_NULLPTR));
        label_5->setText(QApplication::translate("Settings", "Window Function", Q_NULLPTR));
        comboBoxWindowFunction->clear();
        comboBoxWindowFunction->insertItems(0, QStringList()
         << QApplication::translate("Settings", "Rectangular Window (None)", Q_NULLPTR)
         << QApplication::translate("Settings", "Hanning Window", Q_NULLPTR)
         << QApplication::translate("Settings", "Blackman-Harris", Q_NULLPTR)
        );
        buttonQuit->setText(QApplication::translate("Settings", "Quit", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Settings: public Ui_Settings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_H

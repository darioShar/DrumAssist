/********************************************************************************
** Form generated from reading UI file 'recordandcorrect.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECORDANDCORRECT_H
#define UI_RECORDANDCORRECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_RecordAndCorrect
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBoxMCState;
    QVBoxLayout *verticalLayout_8;
    QVBoxLayout *verticalLayout_5;
    QLabel *labelPort;
    QVBoxLayout *verticalLayout_6;
    QGroupBox *groupBoxVolume;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *layoutInput;
    QGroupBox *groupBoxSpectrum;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout;
    QFrame *frameSpectrum;
    QHBoxLayout *horizontalLayout;
    QLabel *label_4;
    QLabel *label_6;
    QHBoxLayout *horizontalLayout_3;
    QLCDNumber *lcdFundamental;
    QLCDNumber *lcdDC;
    QVBoxLayout *verticalLayout_7;
    QLabel *labelCorrections;
    QLabel *labelHarmonic;
    QPushButton *buttonReloadProtocol;
    QLabel *label;
    QProgressBar *progressBarCC0;
    QLabel *label_5;
    QProgressBar *progressBarCC1;
    QPushButton *buttonQuit;

    void setupUi(QDialog *RecordAndCorrect)
    {
        if (RecordAndCorrect->objectName().isEmpty())
            RecordAndCorrect->setObjectName(QStringLiteral("RecordAndCorrect"));
        RecordAndCorrect->setWindowModality(Qt::NonModal);
        RecordAndCorrect->resize(1278, 629);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RecordAndCorrect->sizePolicy().hasHeightForWidth());
        RecordAndCorrect->setSizePolicy(sizePolicy);
        RecordAndCorrect->setMinimumSize(QSize(550, 500));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/Icons/Microphone.png"), QSize(), QIcon::Normal, QIcon::Off);
        RecordAndCorrect->setWindowIcon(icon);
        verticalLayout_2 = new QVBoxLayout(RecordAndCorrect);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setSizeConstraint(QLayout::SetMaximumSize);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetMaximumSize);
        groupBoxMCState = new QGroupBox(RecordAndCorrect);
        groupBoxMCState->setObjectName(QStringLiteral("groupBoxMCState"));
        verticalLayout_8 = new QVBoxLayout(groupBoxMCState);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        labelPort = new QLabel(groupBoxMCState);
        labelPort->setObjectName(QStringLiteral("labelPort"));

        verticalLayout_5->addWidget(labelPort);


        verticalLayout_8->addLayout(verticalLayout_5);


        horizontalLayout_2->addWidget(groupBoxMCState);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        groupBoxVolume = new QGroupBox(RecordAndCorrect);
        groupBoxVolume->setObjectName(QStringLiteral("groupBoxVolume"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBoxVolume->sizePolicy().hasHeightForWidth());
        groupBoxVolume->setSizePolicy(sizePolicy1);
        groupBoxVolume->setMinimumSize(QSize(0, 100));
        groupBoxVolume->setMaximumSize(QSize(16777215, 150));
        verticalLayout_4 = new QVBoxLayout(groupBoxVolume);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        layoutInput = new QVBoxLayout();
        layoutInput->setObjectName(QStringLiteral("layoutInput"));

        verticalLayout_4->addLayout(layoutInput);


        verticalLayout_6->addWidget(groupBoxVolume);

        groupBoxSpectrum = new QGroupBox(RecordAndCorrect);
        groupBoxSpectrum->setObjectName(QStringLiteral("groupBoxSpectrum"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBoxSpectrum->sizePolicy().hasHeightForWidth());
        groupBoxSpectrum->setSizePolicy(sizePolicy2);
        groupBoxSpectrum->setMinimumSize(QSize(900, 200));
        verticalLayout_3 = new QVBoxLayout(groupBoxSpectrum);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        frameSpectrum = new QFrame(groupBoxSpectrum);
        frameSpectrum->setObjectName(QStringLiteral("frameSpectrum"));
        sizePolicy2.setHeightForWidth(frameSpectrum->sizePolicy().hasHeightForWidth());
        frameSpectrum->setSizePolicy(sizePolicy2);
        frameSpectrum->setMinimumSize(QSize(980, 379));
        frameSpectrum->setFrameShape(QFrame::StyledPanel);
        frameSpectrum->setFrameShadow(QFrame::Raised);

        verticalLayout->addWidget(frameSpectrum);


        verticalLayout_3->addLayout(verticalLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_4 = new QLabel(groupBoxSpectrum);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout->addWidget(label_4);

        label_6 = new QLabel(groupBoxSpectrum);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout->addWidget(label_6);


        verticalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        lcdFundamental = new QLCDNumber(groupBoxSpectrum);
        lcdFundamental->setObjectName(QStringLiteral("lcdFundamental"));
        lcdFundamental->setFrameShape(QFrame::Box);
        lcdFundamental->setFrameShadow(QFrame::Sunken);
        lcdFundamental->setSmallDecimalPoint(false);
        lcdFundamental->setDigitCount(5);
        lcdFundamental->setSegmentStyle(QLCDNumber::Flat);

        horizontalLayout_3->addWidget(lcdFundamental);

        lcdDC = new QLCDNumber(groupBoxSpectrum);
        lcdDC->setObjectName(QStringLiteral("lcdDC"));
        lcdDC->setFrameShadow(QFrame::Sunken);
        lcdDC->setSegmentStyle(QLCDNumber::Flat);

        horizontalLayout_3->addWidget(lcdDC);


        verticalLayout_3->addLayout(horizontalLayout_3);


        verticalLayout_6->addWidget(groupBoxSpectrum);


        horizontalLayout_2->addLayout(verticalLayout_6);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        labelCorrections = new QLabel(RecordAndCorrect);
        labelCorrections->setObjectName(QStringLiteral("labelCorrections"));
        labelCorrections->setMaximumSize(QSize(16777215, 50));

        verticalLayout_7->addWidget(labelCorrections);

        labelHarmonic = new QLabel(RecordAndCorrect);
        labelHarmonic->setObjectName(QStringLiteral("labelHarmonic"));

        verticalLayout_7->addWidget(labelHarmonic);

        buttonReloadProtocol = new QPushButton(RecordAndCorrect);
        buttonReloadProtocol->setObjectName(QStringLiteral("buttonReloadProtocol"));
        buttonReloadProtocol->setCursor(QCursor(Qt::PointingHandCursor));
        buttonReloadProtocol->setAutoDefault(false);

        verticalLayout_7->addWidget(buttonReloadProtocol);

        label = new QLabel(RecordAndCorrect);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout_7->addWidget(label);

        progressBarCC0 = new QProgressBar(RecordAndCorrect);
        progressBarCC0->setObjectName(QStringLiteral("progressBarCC0"));
        progressBarCC0->setLayoutDirection(Qt::LeftToRight);
        progressBarCC0->setValue(24);
        progressBarCC0->setAlignment(Qt::AlignCenter);
        progressBarCC0->setOrientation(Qt::Vertical);

        verticalLayout_7->addWidget(progressBarCC0);

        label_5 = new QLabel(RecordAndCorrect);
        label_5->setObjectName(QStringLiteral("label_5"));

        verticalLayout_7->addWidget(label_5);

        progressBarCC1 = new QProgressBar(RecordAndCorrect);
        progressBarCC1->setObjectName(QStringLiteral("progressBarCC1"));
        progressBarCC1->setValue(24);
        progressBarCC1->setOrientation(Qt::Vertical);

        verticalLayout_7->addWidget(progressBarCC1);


        horizontalLayout_2->addLayout(verticalLayout_7);


        verticalLayout_2->addLayout(horizontalLayout_2);

        buttonQuit = new QPushButton(RecordAndCorrect);
        buttonQuit->setObjectName(QStringLiteral("buttonQuit"));
        buttonQuit->setCursor(QCursor(Qt::PointingHandCursor));
        buttonQuit->setAutoDefault(false);

        verticalLayout_2->addWidget(buttonQuit, 0, Qt::AlignRight);


        retranslateUi(RecordAndCorrect);

        QMetaObject::connectSlotsByName(RecordAndCorrect);
    } // setupUi

    void retranslateUi(QDialog *RecordAndCorrect)
    {
        RecordAndCorrect->setWindowTitle(QApplication::translate("RecordAndCorrect", "Recording and Autocorrecting", Q_NULLPTR));
        groupBoxMCState->setTitle(QApplication::translate("RecordAndCorrect", "Microcontroler State", Q_NULLPTR));
        labelPort->setText(QApplication::translate("RecordAndCorrect", "<html><head/><body><p><span style=\" color:#ff0000;\">No info</span></p></body></html>", Q_NULLPTR));
        groupBoxVolume->setTitle(QApplication::translate("RecordAndCorrect", "Volume", Q_NULLPTR));
        groupBoxSpectrum->setTitle(QApplication::translate("RecordAndCorrect", "Frequency Spectrum", Q_NULLPTR));
        label_4->setText(QApplication::translate("RecordAndCorrect", "Fundamental (Hz) :", Q_NULLPTR));
        label_6->setText(QApplication::translate("RecordAndCorrect", "DC Value (multiplied by 1000)", Q_NULLPTR));
        labelCorrections->setText(QApplication::translate("RecordAndCorrect", "Corrections :", Q_NULLPTR));
        labelHarmonic->setText(QString());
        buttonReloadProtocol->setText(QApplication::translate("RecordAndCorrect", "Reload Database", Q_NULLPTR));
        label->setText(QApplication::translate("RecordAndCorrect", "Correlation 1", Q_NULLPTR));
        label_5->setText(QApplication::translate("RecordAndCorrect", "Correlation 2", Q_NULLPTR));
        buttonQuit->setText(QApplication::translate("RecordAndCorrect", "Quit", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class RecordAndCorrect: public Ui_RecordAndCorrect {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECORDANDCORRECT_H

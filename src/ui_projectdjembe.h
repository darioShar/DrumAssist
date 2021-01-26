/********************************************************************************
** Form generated from reading UI file 'projectdjembe.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROJECTDJEMBE_H
#define UI_PROJECTDJEMBE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProjectDjembe
{
public:
    QVBoxLayout *verticalLayout_11;
    QTabWidget *tabChoice;
    QWidget *record;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *groupBoxAutoSequence;
    QVBoxLayout *verticalLayout_12;
    QSpacerItem *verticalSpacer;
    QPushButton *buttonStart_AutoCorrect;
    QLabel *labelStatus_AutoCorrect;
    QSpacerItem *verticalSpacer_5;
    QFrame *line_6;
    QHBoxLayout *horizontalLayout_15;
    QSpacerItem *horizontalSpacer_6;
    QLabel *labelMic;
    QSpacerItem *horizontalSpacer_7;
    QLabel *labelMCStatusRecord;
    QWidget *showInfo;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_21;
    QLabel *label;
    QFrame *frameInfo;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonSettings;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *buttonQuit;

    void setupUi(QDialog *ProjectDjembe)
    {
        if (ProjectDjembe->objectName().isEmpty())
            ProjectDjembe->setObjectName(QStringLiteral("ProjectDjembe"));
        ProjectDjembe->resize(1017, 517);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/Icons/oie_transparent (1).png"), QSize(), QIcon::Normal, QIcon::Off);
        ProjectDjembe->setWindowIcon(icon);
        ProjectDjembe->setWindowOpacity(1);
        verticalLayout_11 = new QVBoxLayout(ProjectDjembe);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        tabChoice = new QTabWidget(ProjectDjembe);
        tabChoice->setObjectName(QStringLiteral("tabChoice"));
        record = new QWidget();
        record->setObjectName(QStringLiteral("record"));
        horizontalLayout_2 = new QHBoxLayout(record);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        groupBoxAutoSequence = new QGroupBox(record);
        groupBoxAutoSequence->setObjectName(QStringLiteral("groupBoxAutoSequence"));
        verticalLayout_12 = new QVBoxLayout(groupBoxAutoSequence);
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_12->addItem(verticalSpacer);

        buttonStart_AutoCorrect = new QPushButton(groupBoxAutoSequence);
        buttonStart_AutoCorrect->setObjectName(QStringLiteral("buttonStart_AutoCorrect"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(buttonStart_AutoCorrect->sizePolicy().hasHeightForWidth());
        buttonStart_AutoCorrect->setSizePolicy(sizePolicy);
        buttonStart_AutoCorrect->setCursor(QCursor(Qt::PointingHandCursor));
        buttonStart_AutoCorrect->setCheckable(false);
        buttonStart_AutoCorrect->setAutoDefault(false);

        verticalLayout_12->addWidget(buttonStart_AutoCorrect);

        labelStatus_AutoCorrect = new QLabel(groupBoxAutoSequence);
        labelStatus_AutoCorrect->setObjectName(QStringLiteral("labelStatus_AutoCorrect"));

        verticalLayout_12->addWidget(labelStatus_AutoCorrect);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_12->addItem(verticalSpacer_5);


        horizontalLayout_3->addWidget(groupBoxAutoSequence);


        verticalLayout->addLayout(horizontalLayout_3);

        line_6 = new QFrame(record);
        line_6->setObjectName(QStringLiteral("line_6"));
        line_6->setFrameShape(QFrame::HLine);
        line_6->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_6);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer_6);

        labelMic = new QLabel(record);
        labelMic->setObjectName(QStringLiteral("labelMic"));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        QBrush brush1(QColor(120, 120, 120, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush1);
        labelMic->setPalette(palette);
        labelMic->setFrameShape(QFrame::NoFrame);
        labelMic->setFrameShadow(QFrame::Sunken);
        labelMic->setLineWidth(1);

        horizontalLayout_15->addWidget(labelMic);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer_7);


        verticalLayout->addLayout(horizontalLayout_15);

        labelMCStatusRecord = new QLabel(record);
        labelMCStatusRecord->setObjectName(QStringLiteral("labelMCStatusRecord"));

        verticalLayout->addWidget(labelMCStatusRecord, 0, Qt::AlignHCenter);


        horizontalLayout_2->addLayout(verticalLayout);

        tabChoice->addTab(record, QString());
        showInfo = new QWidget();
        showInfo->setObjectName(QStringLiteral("showInfo"));
        verticalLayout_7 = new QVBoxLayout(showInfo);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        horizontalLayout_21 = new QHBoxLayout();
        horizontalLayout_21->setObjectName(QStringLiteral("horizontalLayout_21"));
        label = new QLabel(showInfo);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_21->addWidget(label);

        frameInfo = new QFrame(showInfo);
        frameInfo->setObjectName(QStringLiteral("frameInfo"));
        frameInfo->setFrameShape(QFrame::StyledPanel);
        frameInfo->setFrameShadow(QFrame::Raised);

        horizontalLayout_21->addWidget(frameInfo);


        verticalLayout_7->addLayout(horizontalLayout_21);

        tabChoice->addTab(showInfo, QString());

        verticalLayout_11->addWidget(tabChoice);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        buttonSettings = new QPushButton(ProjectDjembe);
        buttonSettings->setObjectName(QStringLiteral("buttonSettings"));
        buttonSettings->setCursor(QCursor(Qt::PointingHandCursor));
        buttonSettings->setAutoDefault(false);

        horizontalLayout->addWidget(buttonSettings);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        buttonQuit = new QPushButton(ProjectDjembe);
        buttonQuit->setObjectName(QStringLiteral("buttonQuit"));
        buttonQuit->setCursor(QCursor(Qt::PointingHandCursor));
        buttonQuit->setAutoDefault(false);
        buttonQuit->setFlat(false);

        horizontalLayout->addWidget(buttonQuit);


        verticalLayout_11->addLayout(horizontalLayout);


        retranslateUi(ProjectDjembe);

        tabChoice->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ProjectDjembe);
    } // setupUi

    void retranslateUi(QDialog *ProjectDjembe)
    {
        ProjectDjembe->setWindowTitle(QApplication::translate("ProjectDjembe", "Drum'Assist", Q_NULLPTR));
        groupBoxAutoSequence->setTitle(QApplication::translate("ProjectDjembe", "Auto-Correcting Sequence", Q_NULLPTR));
        buttonStart_AutoCorrect->setText(QApplication::translate("ProjectDjembe", "Start Sequence", Q_NULLPTR));
        labelStatus_AutoCorrect->setText(QApplication::translate("ProjectDjembe", "<html><head/><body><p><span style=\" color:#ff0000;\">Status : Stopped</span></p></body></html>", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        labelMic->setToolTip(QApplication::translate("ProjectDjembe", "Microphone status", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        labelMic->setText(QApplication::translate("ProjectDjembe", "<html><head/><body><p><span style=\" color:#ff0000;\">Microphone not ready !</span></p></body></html>", Q_NULLPTR));
        labelMCStatusRecord->setText(QApplication::translate("ProjectDjembe", "<html><head/><body><p><span style=\" color:#ff0000;\">MicroControler not connected !</span></p></body></html>", Q_NULLPTR));
        tabChoice->setTabText(tabChoice->indexOf(record), QApplication::translate("ProjectDjembe", "Record", Q_NULLPTR));
        label->setText(QApplication::translate("ProjectDjembe", "<html><head/><body><p><span style=\" color:#ff0000;\">No information to be displayed</span></p></body></html>", Q_NULLPTR));
        tabChoice->setTabText(tabChoice->indexOf(showInfo), QApplication::translate("ProjectDjembe", "ShowCompleteInfo", Q_NULLPTR));
        buttonSettings->setText(QApplication::translate("ProjectDjembe", "Settings", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        buttonQuit->setToolTip(QApplication::translate("ProjectDjembe", "Quit program", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonQuit->setText(QApplication::translate("ProjectDjembe", "Quit", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ProjectDjembe: public Ui_ProjectDjembe {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROJECTDJEMBE_H

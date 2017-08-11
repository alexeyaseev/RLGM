/********************************************************************************
** Form generated from reading UI file 'ControlPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROLPANEL_H
#define UI_CONTROLPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ControlPanel
{
public:
    QGroupBox *groupBox;
    QSlider *horizontalSlider;
    QSlider *horizontalSlider_7;
    QLabel *label_7;
    QSlider *horizontalSlider_3;
    QLabel *label_3;
    QSlider *horizontalSlider_2;
    QLabel *label_2;
    QSlider *horizontalSlider_4;
    QLabel *label_4;
    QLabel *label_5;
    QSlider *horizontalSlider_6;
    QSlider *horizontalSlider_5;
    QLabel *label_6;
    QCheckBox *checkBox;
    QLabel *label;
    QGroupBox *groupBox_3;
    QSlider *horizontalSlider_15;
    QSlider *horizontalSlider_16;
    QLabel *label_15;
    QSlider *horizontalSlider_18;
    QLabel *label_17;
    QSlider *horizontalSlider_19;
    QLabel *label_18;
    QCheckBox *checkBox_3;
    QLabel *label_21;
    QPushButton *pushButton;
    QGroupBox *groupBox_4;
    QCheckBox *checkBox_4;
    QLabel *label_19;
    QLabel *label_20;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QLabel *label_22;
    QPushButton *pushButton_4;
    QGroupBox *groupBox_5;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QGroupBox *groupBox_6;
    QRadioButton *radioButton_3;
    QRadioButton *radioButton_4;
    QGroupBox *groupBox_7;
    QSlider *horizontalSlider_20;
    QCheckBox *checkBox_5;
    QCheckBox *checkBox_6;
    QLabel *label_23;
    QLabel *label_24;
    QCheckBox *checkBox_7;
    QCheckBox *checkBox_8;
    QLabel *label_25;
    QLabel *label_26;
    QSpinBox *spinBox;
    QLabel *label_27;
    QSpinBox *spinBox_2;
    QLabel *label_28;
    QPushButton *pushButton_5;
    QLabel *label_29;
    QPushButton *pushButton_6;

    void setupUi(QWidget *ControlPanel)
    {
        if (ControlPanel->objectName().isEmpty())
            ControlPanel->setObjectName(QStringLiteral("ControlPanel"));
        ControlPanel->resize(277, 772);
        groupBox = new QGroupBox(ControlPanel);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 0, 251, 191));
        horizontalSlider = new QSlider(groupBox);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(120, 20, 120, 22));
        horizontalSlider->setOrientation(Qt::Horizontal);
        horizontalSlider_7 = new QSlider(groupBox);
        horizontalSlider_7->setObjectName(QStringLiteral("horizontalSlider_7"));
        horizontalSlider_7->setGeometry(QRect(120, 40, 120, 22));
        horizontalSlider_7->setOrientation(Qt::Horizontal);
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(30, 40, 81, 20));
        QFont font;
        font.setUnderline(true);
        label_7->setFont(font);
        label_7->setFrameShape(QFrame::NoFrame);
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_7->setWordWrap(false);
        horizontalSlider_3 = new QSlider(groupBox);
        horizontalSlider_3->setObjectName(QStringLiteral("horizontalSlider_3"));
        horizontalSlider_3->setGeometry(QRect(120, 60, 120, 22));
        horizontalSlider_3->setOrientation(Qt::Horizontal);
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(60, 60, 51, 19));
        label_3->setFont(font);
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        horizontalSlider_2 = new QSlider(groupBox);
        horizontalSlider_2->setObjectName(QStringLiteral("horizontalSlider_2"));
        horizontalSlider_2->setGeometry(QRect(120, 80, 120, 22));
        horizontalSlider_2->setOrientation(Qt::Horizontal);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(80, 80, 31, 19));
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        horizontalSlider_4 = new QSlider(groupBox);
        horizontalSlider_4->setObjectName(QStringLiteral("horizontalSlider_4"));
        horizontalSlider_4->setGeometry(QRect(120, 100, 120, 22));
        horizontalSlider_4->setOrientation(Qt::Horizontal);
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(0, 100, 111, 19));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(0, 120, 111, 20));
        label_5->setFont(font);
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        horizontalSlider_6 = new QSlider(groupBox);
        horizontalSlider_6->setObjectName(QStringLiteral("horizontalSlider_6"));
        horizontalSlider_6->setGeometry(QRect(120, 120, 120, 22));
        horizontalSlider_6->setOrientation(Qt::Horizontal);
        horizontalSlider_5 = new QSlider(groupBox);
        horizontalSlider_5->setObjectName(QStringLiteral("horizontalSlider_5"));
        horizontalSlider_5->setGeometry(QRect(120, 140, 120, 22));
        horizontalSlider_5->setOrientation(Qt::Horizontal);
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(0, 140, 111, 19));
        label_6->setFont(font);
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        checkBox = new QCheckBox(groupBox);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(10, 160, 131, 23));
        checkBox->setChecked(true);
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 20, 71, 20));
        label->setFont(font);
        label->setLayoutDirection(Qt::LeftToRight);
        label->setAutoFillBackground(false);
        label->setScaledContents(false);
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label->setWordWrap(false);
        groupBox_3 = new QGroupBox(ControlPanel);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 190, 251, 141));
        horizontalSlider_15 = new QSlider(groupBox_3);
        horizontalSlider_15->setObjectName(QStringLiteral("horizontalSlider_15"));
        horizontalSlider_15->setGeometry(QRect(120, 20, 120, 22));
        horizontalSlider_15->setOrientation(Qt::Horizontal);
        horizontalSlider_16 = new QSlider(groupBox_3);
        horizontalSlider_16->setObjectName(QStringLiteral("horizontalSlider_16"));
        horizontalSlider_16->setGeometry(QRect(120, 40, 120, 22));
        horizontalSlider_16->setOrientation(Qt::Horizontal);
        label_15 = new QLabel(groupBox_3);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(0, 20, 111, 20));
        label_15->setFont(font);
        label_15->setFrameShape(QFrame::NoFrame);
        label_15->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_15->setWordWrap(false);
        horizontalSlider_18 = new QSlider(groupBox_3);
        horizontalSlider_18->setObjectName(QStringLiteral("horizontalSlider_18"));
        horizontalSlider_18->setGeometry(QRect(120, 60, 120, 22));
        horizontalSlider_18->setOrientation(Qt::Horizontal);
        label_17 = new QLabel(groupBox_3);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(80, 60, 31, 19));
        label_17->setFont(font);
        label_17->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        horizontalSlider_19 = new QSlider(groupBox_3);
        horizontalSlider_19->setObjectName(QStringLiteral("horizontalSlider_19"));
        horizontalSlider_19->setGeometry(QRect(120, 80, 120, 22));
        horizontalSlider_19->setOrientation(Qt::Horizontal);
        label_18 = new QLabel(groupBox_3);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(0, 80, 111, 19));
        label_18->setFont(font);
        label_18->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        checkBox_3 = new QCheckBox(groupBox_3);
        checkBox_3->setObjectName(QStringLiteral("checkBox_3"));
        checkBox_3->setGeometry(QRect(10, 110, 131, 23));
        label_21 = new QLabel(groupBox_3);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setGeometry(QRect(40, 40, 71, 20));
        label_21->setFont(font);
        label_21->setLayoutDirection(Qt::LeftToRight);
        label_21->setAutoFillBackground(false);
        label_21->setScaledContents(false);
        label_21->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_21->setWordWrap(false);
        pushButton = new QPushButton(groupBox_3);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(140, 110, 101, 31));
        groupBox_4 = new QGroupBox(ControlPanel);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 330, 251, 131));
        checkBox_4 = new QCheckBox(groupBox_4);
        checkBox_4->setObjectName(QStringLiteral("checkBox_4"));
        checkBox_4->setGeometry(QRect(10, 100, 131, 23));
        label_19 = new QLabel(groupBox_4);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(-10, 20, 91, 20));
        label_19->setFont(font);
        label_19->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_20 = new QLabel(groupBox_4);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setGeometry(QRect(-10, 50, 91, 20));
        label_20->setFont(font);
        label_20->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushButton_2 = new QPushButton(groupBox_4);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(90, 10, 41, 34));
        pushButton_2->setAutoFillBackground(false);
        pushButton_2->setFlat(false);
        pushButton_3 = new QPushButton(groupBox_4);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(90, 40, 41, 34));
        label_22 = new QLabel(groupBox_4);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setGeometry(QRect(-10, 80, 91, 20));
        label_22->setFont(font);
        label_22->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushButton_4 = new QPushButton(groupBox_4);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(90, 70, 41, 34));
        groupBox_5 = new QGroupBox(ControlPanel);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(10, 460, 251, 71));
        radioButton = new QRadioButton(groupBox_5);
        radioButton->setObjectName(QStringLiteral("radioButton"));
        radioButton->setGeometry(QRect(10, 20, 119, 23));
        radioButton_2 = new QRadioButton(groupBox_5);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));
        radioButton_2->setGeometry(QRect(10, 40, 201, 23));
        groupBox_6 = new QGroupBox(ControlPanel);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        groupBox_6->setGeometry(QRect(10, 530, 251, 71));
        radioButton_3 = new QRadioButton(groupBox_6);
        radioButton_3->setObjectName(QStringLiteral("radioButton_3"));
        radioButton_3->setGeometry(QRect(10, 20, 119, 23));
        radioButton_4 = new QRadioButton(groupBox_6);
        radioButton_4->setObjectName(QStringLiteral("radioButton_4"));
        radioButton_4->setGeometry(QRect(10, 40, 201, 23));
        groupBox_7 = new QGroupBox(ControlPanel);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));
        groupBox_7->setGeometry(QRect(10, 600, 251, 101));
        horizontalSlider_20 = new QSlider(groupBox_7);
        horizontalSlider_20->setObjectName(QStringLiteral("horizontalSlider_20"));
        horizontalSlider_20->setGeometry(QRect(120, 70, 120, 22));
        horizontalSlider_20->setOrientation(Qt::Horizontal);
        checkBox_5 = new QCheckBox(groupBox_7);
        checkBox_5->setObjectName(QStringLiteral("checkBox_5"));
        checkBox_5->setGeometry(QRect(80, 20, 21, 23));
        checkBox_6 = new QCheckBox(groupBox_7);
        checkBox_6->setObjectName(QStringLiteral("checkBox_6"));
        checkBox_6->setGeometry(QRect(190, 20, 21, 23));
        label_23 = new QLabel(ControlPanel);
        label_23->setObjectName(QStringLiteral("label_23"));
        label_23->setGeometry(QRect(10, 620, 61, 20));
        label_23->setFont(font);
        label_23->setFrameShape(QFrame::NoFrame);
        label_23->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_23->setWordWrap(false);
        label_24 = new QLabel(ControlPanel);
        label_24->setObjectName(QStringLiteral("label_24"));
        label_24->setGeometry(QRect(120, 620, 61, 20));
        label_24->setFont(font);
        label_24->setFrameShape(QFrame::NoFrame);
        label_24->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_24->setWordWrap(false);
        checkBox_7 = new QCheckBox(ControlPanel);
        checkBox_7->setObjectName(QStringLiteral("checkBox_7"));
        checkBox_7->setGeometry(QRect(90, 640, 21, 23));
        checkBox_8 = new QCheckBox(ControlPanel);
        checkBox_8->setObjectName(QStringLiteral("checkBox_8"));
        checkBox_8->setGeometry(QRect(200, 640, 21, 23));
        label_25 = new QLabel(ControlPanel);
        label_25->setObjectName(QStringLiteral("label_25"));
        label_25->setGeometry(QRect(120, 640, 61, 20));
        label_25->setFont(font);
        label_25->setFrameShape(QFrame::NoFrame);
        label_25->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_25->setWordWrap(false);
        label_26 = new QLabel(ControlPanel);
        label_26->setObjectName(QStringLiteral("label_26"));
        label_26->setGeometry(QRect(10, 640, 61, 20));
        label_26->setFont(font);
        label_26->setFrameShape(QFrame::NoFrame);
        label_26->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_26->setWordWrap(false);
        spinBox = new QSpinBox(ControlPanel);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setGeometry(QRect(60, 710, 41, 25));
        label_27 = new QLabel(ControlPanel);
        label_27->setObjectName(QStringLiteral("label_27"));
        label_27->setGeometry(QRect(10, 710, 51, 20));
        label_27->setFont(font);
        label_27->setFrameShape(QFrame::NoFrame);
        label_27->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_27->setWordWrap(false);
        spinBox_2 = new QSpinBox(ControlPanel);
        spinBox_2->setObjectName(QStringLiteral("spinBox_2"));
        spinBox_2->setGeometry(QRect(60, 740, 41, 25));
        label_28 = new QLabel(ControlPanel);
        label_28->setObjectName(QStringLiteral("label_28"));
        label_28->setGeometry(QRect(10, 740, 51, 20));
        label_28->setFont(font);
        label_28->setFrameShape(QFrame::NoFrame);
        label_28->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_28->setWordWrap(false);
        pushButton_5 = new QPushButton(ControlPanel);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(210, 710, 61, 51));
        label_29 = new QLabel(ControlPanel);
        label_29->setObjectName(QStringLiteral("label_29"));
        label_29->setGeometry(QRect(0, 670, 121, 20));
        label_29->setFont(font);
        label_29->setFrameShape(QFrame::NoFrame);
        label_29->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_29->setWordWrap(false);
        pushButton_6 = new QPushButton(ControlPanel);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setGeometry(QRect(120, 710, 61, 51));

        retranslateUi(ControlPanel);

        QMetaObject::connectSlotsByName(ControlPanel);
    } // setupUi

    void retranslateUi(QWidget *ControlPanel)
    {
        ControlPanel->setWindowTitle(QApplication::translate("ControlPanel", "ControlPanel", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("ControlPanel", "8 \320\261\320\270\321\202\320\272\320\260", Q_NULLPTR));
        label_7->setText(QApplication::translate("ControlPanel", "\320\232\320\276\320\275\321\202\321\200\320\260\321\201\321\202", Q_NULLPTR));
        label_3->setText(QApplication::translate("ControlPanel", "\320\223\320\260\320\274\320\274\320\260", Q_NULLPTR));
        label_2->setText(QApplication::translate("ControlPanel", "\320\242\320\276\320\275", Q_NULLPTR));
        label_4->setText(QApplication::translate("ControlPanel", "\320\235\320\260\321\201\321\213\321\211\320\265\320\275\320\275\320\276\321\201\321\202\321\214", Q_NULLPTR));
        label_5->setText(QApplication::translate("ControlPanel", "\320\235\320\260\321\207. \320\267\320\275\320\260\321\207\320\265\320\275\320\270\320\265", Q_NULLPTR));
        label_6->setText(QApplication::translate("ControlPanel", "\320\232\320\276\320\275. \320\267\320\275\320\260\321\207\320\265\320\275\320\270\320\265", Q_NULLPTR));
        checkBox->setText(QApplication::translate("ControlPanel", "\320\236\321\202\320\276\320\261\321\200\320\260\320\266\320\260\321\202\321\214", Q_NULLPTR));
        label->setText(QApplication::translate("ControlPanel", "\320\257\321\200\320\272\320\276\321\201\321\202\321\214", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("ControlPanel", "\320\245\320\262\320\276\321\201\321\202\321\213", Q_NULLPTR));
        label_15->setText(QApplication::translate("ControlPanel", "\320\224\320\273\320\270\321\202\320\265\320\273\321\214\320\275\320\276\321\201\321\202\321\214", Q_NULLPTR));
        label_17->setText(QApplication::translate("ControlPanel", "\320\242\320\276\320\275", Q_NULLPTR));
        label_18->setText(QApplication::translate("ControlPanel", "\320\235\320\260\321\201\321\213\321\211\320\265\320\275\320\275\320\276\321\201\321\202\321\214", Q_NULLPTR));
        checkBox_3->setText(QApplication::translate("ControlPanel", "\320\236\321\202\320\276\320\261\321\200\320\260\320\266\320\260\321\202\321\214", Q_NULLPTR));
        label_21->setText(QApplication::translate("ControlPanel", "\320\257\321\200\320\272\320\276\321\201\321\202\321\214", Q_NULLPTR));
        pushButton->setText(QApplication::translate("ControlPanel", "\320\236\321\207\320\270\321\201\321\202\320\272\320\260", Q_NULLPTR));
        groupBox_4->setTitle(QApplication::translate("ControlPanel", "2 \320\261\320\270\321\202\320\272\320\260", Q_NULLPTR));
        checkBox_4->setText(QApplication::translate("ControlPanel", "\320\236\321\202\320\276\320\261\321\200\320\260\320\266\320\260\321\202\321\214", Q_NULLPTR));
        label_19->setText(QApplication::translate("ControlPanel", "\320\243\321\200\320\276\320\262\320\265\320\275\321\214 1", Q_NULLPTR));
        label_20->setText(QApplication::translate("ControlPanel", "\320\243\321\200\320\276\320\262\320\265\320\275\321\214 2", Q_NULLPTR));
        pushButton_2->setText(QString());
        pushButton_3->setText(QString());
        label_22->setText(QApplication::translate("ControlPanel", "\320\243\321\200\320\276\320\262\320\265\320\275\321\214 3", Q_NULLPTR));
        pushButton_4->setText(QString());
        groupBox_5->setTitle(QApplication::translate("ControlPanel", "\320\240\320\265\320\266\320\270\320\274 \321\201\320\272\320\273\320\265\320\271\320\272\320\270 8 \320\261\320\270\321\202\320\276\320\272", Q_NULLPTR));
        radioButton->setText(QApplication::translate("ControlPanel", "\320\234\320\260\320\272\321\201\320\270\320\274\321\203\320\274", Q_NULLPTR));
        radioButton_2->setText(QApplication::translate("ControlPanel", "\320\233\320\270\320\275\320\265\320\271\320\275\320\260\321\217 \320\272\320\276\320\274\320\261\320\270\320\275\320\260\321\206\320\270\321\217", Q_NULLPTR));
        groupBox_6->setTitle(QApplication::translate("ControlPanel", "\320\240\320\265\320\266\320\270\320\274 \321\204\320\270\320\273\321\214\321\202\321\200\320\260\321\206\320\270\320\270 \321\202\320\265\320\272\321\201\321\202\321\203\321\200", Q_NULLPTR));
        radioButton_3->setText(QApplication::translate("ControlPanel", "\320\242\320\276\321\207\320\265\321\207\320\275\320\260\321\217", Q_NULLPTR));
        radioButton_4->setText(QApplication::translate("ControlPanel", "\320\221\320\270\320\273\320\270\320\275\320\265\320\271\320\275\320\260\321\217", Q_NULLPTR));
        groupBox_7->setTitle(QApplication::translate("ControlPanel", "\320\241\320\273\320\276\320\270", Q_NULLPTR));
        checkBox_5->setText(QString());
        checkBox_6->setText(QString());
        label_23->setText(QApplication::translate("ControlPanel", "8 \320\261\320\270\321\202\320\272\320\260", Q_NULLPTR));
        label_24->setText(QApplication::translate("ControlPanel", "2 \320\261\320\270\321\202\320\272\320\260", Q_NULLPTR));
        checkBox_7->setText(QString());
        checkBox_8->setText(QString());
        label_25->setText(QApplication::translate("ControlPanel", "\320\245\320\262\320\276\321\201\321\202\321\213", Q_NULLPTR));
        label_26->setText(QApplication::translate("ControlPanel", "\320\232\320\260\321\200\321\202\320\260", Q_NULLPTR));
        spinBox->setSuffix(QString());
        label_27->setText(QApplication::translate("ControlPanel", "\320\236\320\272\320\275\320\276", Q_NULLPTR));
        spinBox_2->setSuffix(QString());
        label_28->setText(QApplication::translate("ControlPanel", "\320\240\320\260\320\264\320\260\321\200", Q_NULLPTR));
        pushButton_5->setText(QApplication::translate("ControlPanel", "\320\222\321\213\321\205\320\276\320\264", Q_NULLPTR));
        label_29->setText(QApplication::translate("ControlPanel", "\320\232\320\260\321\200\321\202\320\260+8\320\261\320\270\321\202\320\272\320\260", Q_NULLPTR));
        pushButton_6->setText(QApplication::translate("ControlPanel", "\320\237\320\260\321\203\320\267\320\260", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ControlPanel: public Ui_ControlPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLPANEL_H

/********************************************************************************
** Form generated from reading UI file 'gamemode.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEMODE_H
#define UI_GAMEMODE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GameMode
{
public:
    QVBoxLayout *verticalLayout_6;
    QStackedWidget *stackedWidget;
    QWidget *mode_page;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_3;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QLabel *single_lable;
    QPushButton *singleModeBtn;
    QSpacerItem *horizontalSpacer_5;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *net_lable;
    QPushButton *netModeBtn;
    QSpacerItem *horizontalSpacer_4;
    QWidget *room_page;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer;
    QLabel *auto_lable;
    QPushButton *autoBtn;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer_6;
    QWidget *widget_4;
    QVBoxLayout *verticalLayout_4;
    QSpacerItem *verticalSpacer_3;
    QLabel *manual_lable;
    QPushButton *manualBtn;
    QSpacerItem *verticalSpacer_4;
    QSpacerItem *horizontalSpacer_7;
    QWidget *widget_5;
    QVBoxLayout *verticalLayout_5;
    QSpacerItem *verticalSpacer_5;
    QLabel *search_lable;
    QPushButton *searchBtn;
    QSpacerItem *verticalSpacer_6;
    QSpacerItem *horizontalSpacer_2;
    QLabel *information;

    void setupUi(QDialog *GameMode)
    {
        if (GameMode->objectName().isEmpty())
            GameMode->setObjectName(QString::fromUtf8("GameMode"));
        GameMode->resize(596, 489);
        verticalLayout_6 = new QVBoxLayout(GameMode);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        stackedWidget = new QStackedWidget(GameMode);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        mode_page = new QWidget();
        mode_page->setObjectName(QString::fromUtf8("mode_page"));
        horizontalLayout = new QHBoxLayout(mode_page);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        widget = new QWidget(mode_page);
        widget->setObjectName(QString::fromUtf8("widget"));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        single_lable = new QLabel(widget);
        single_lable->setObjectName(QString::fromUtf8("single_lable"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(single_lable->sizePolicy().hasHeightForWidth());
        single_lable->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(single_lable);

        singleModeBtn = new QPushButton(widget);
        singleModeBtn->setObjectName(QString::fromUtf8("singleModeBtn"));

        verticalLayout->addWidget(singleModeBtn);


        horizontalLayout->addWidget(widget);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_5);

        widget_2 = new QWidget(mode_page);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        verticalLayout_2 = new QVBoxLayout(widget_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        net_lable = new QLabel(widget_2);
        net_lable->setObjectName(QString::fromUtf8("net_lable"));
        sizePolicy.setHeightForWidth(net_lable->sizePolicy().hasHeightForWidth());
        net_lable->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(net_lable);

        netModeBtn = new QPushButton(widget_2);
        netModeBtn->setObjectName(QString::fromUtf8("netModeBtn"));

        verticalLayout_2->addWidget(netModeBtn);


        horizontalLayout->addWidget(widget_2);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        stackedWidget->addWidget(mode_page);
        room_page = new QWidget();
        room_page->setObjectName(QString::fromUtf8("room_page"));
        horizontalLayout_2 = new QHBoxLayout(room_page);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        widget_3 = new QWidget(room_page);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        verticalLayout_3 = new QVBoxLayout(widget_3);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalSpacer = new QSpacerItem(20, 184, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        auto_lable = new QLabel(widget_3);
        auto_lable->setObjectName(QString::fromUtf8("auto_lable"));

        verticalLayout_3->addWidget(auto_lable);

        autoBtn = new QPushButton(widget_3);
        autoBtn->setObjectName(QString::fromUtf8("autoBtn"));

        verticalLayout_3->addWidget(autoBtn);

        verticalSpacer_2 = new QSpacerItem(20, 184, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);


        horizontalLayout_2->addWidget(widget_3);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_6);

        widget_4 = new QWidget(room_page);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        verticalLayout_4 = new QVBoxLayout(widget_4);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_3);

        manual_lable = new QLabel(widget_4);
        manual_lable->setObjectName(QString::fromUtf8("manual_lable"));

        verticalLayout_4->addWidget(manual_lable);

        manualBtn = new QPushButton(widget_4);
        manualBtn->setObjectName(QString::fromUtf8("manualBtn"));

        verticalLayout_4->addWidget(manualBtn);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_4);


        horizontalLayout_2->addWidget(widget_4);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_7);

        widget_5 = new QWidget(room_page);
        widget_5->setObjectName(QString::fromUtf8("widget_5"));
        verticalLayout_5 = new QVBoxLayout(widget_5);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_5);

        search_lable = new QLabel(widget_5);
        search_lable->setObjectName(QString::fromUtf8("search_lable"));

        verticalLayout_5->addWidget(search_lable);

        searchBtn = new QPushButton(widget_5);
        searchBtn->setObjectName(QString::fromUtf8("searchBtn"));

        verticalLayout_5->addWidget(searchBtn);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_6);


        horizontalLayout_2->addWidget(widget_5);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        stackedWidget->addWidget(room_page);

        verticalLayout_6->addWidget(stackedWidget);

        information = new QLabel(GameMode);
        information->setObjectName(QString::fromUtf8("information"));

        verticalLayout_6->addWidget(information);


        retranslateUi(GameMode);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(GameMode);
    } // setupUi

    void retranslateUi(QDialog *GameMode)
    {
        GameMode->setWindowTitle(QCoreApplication::translate("GameMode", "Dialog", nullptr));
        single_lable->setText(QString());
        singleModeBtn->setText(QString());
        net_lable->setText(QString());
        netModeBtn->setText(QString());
        auto_lable->setText(QString());
        autoBtn->setText(QString());
        manual_lable->setText(QString());
        manualBtn->setText(QString());
        search_lable->setText(QString());
        searchBtn->setText(QString());
        information->setText(QCoreApplication::translate("GameMode", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GameMode: public Ui_GameMode {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEMODE_H

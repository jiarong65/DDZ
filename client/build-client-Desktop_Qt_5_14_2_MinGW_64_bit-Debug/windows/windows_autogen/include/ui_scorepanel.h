/********************************************************************************
** Form generated from reading UI file 'scorepanel.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCOREPANEL_H
#define UI_SCOREPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScorePanel
{
public:
    QGridLayout *gridLayout;
    QLabel *score3;
    QLabel *score2;
    QLabel *score1;
    QLabel *meScore;
    QLabel *rightScore;
    QLabel *rightTitle;
    QLabel *leftScore;
    QLabel *leftTitle;
    QLabel *meTitle;

    void setupUi(QWidget *ScorePanel)
    {
        if (ScorePanel->objectName().isEmpty())
            ScorePanel->setObjectName(QString::fromUtf8("ScorePanel"));
        ScorePanel->resize(224, 117);
        gridLayout = new QGridLayout(ScorePanel);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        score3 = new QLabel(ScorePanel);
        score3->setObjectName(QString::fromUtf8("score3"));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        score3->setFont(font);
        score3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        score3->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(score3, 3, 2, 1, 1);

        score2 = new QLabel(ScorePanel);
        score2->setObjectName(QString::fromUtf8("score2"));
        score2->setFont(font);
        score2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        score2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(score2, 2, 2, 1, 1);

        score1 = new QLabel(ScorePanel);
        score1->setObjectName(QString::fromUtf8("score1"));
        score1->setFont(font);
        score1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        score1->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(score1, 0, 2, 1, 1);

        meScore = new QLabel(ScorePanel);
        meScore->setObjectName(QString::fromUtf8("meScore"));
        meScore->setFont(font);
        meScore->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        meScore->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(meScore, 2, 1, 1, 1);

        rightScore = new QLabel(ScorePanel);
        rightScore->setObjectName(QString::fromUtf8("rightScore"));
        rightScore->setFont(font);
        rightScore->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        rightScore->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(rightScore, 3, 1, 1, 1);

        rightTitle = new QLabel(ScorePanel);
        rightTitle->setObjectName(QString::fromUtf8("rightTitle"));
        rightTitle->setFont(font);
        rightTitle->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        rightTitle->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(rightTitle, 3, 0, 1, 1);

        leftScore = new QLabel(ScorePanel);
        leftScore->setObjectName(QString::fromUtf8("leftScore"));
        leftScore->setFont(font);
        leftScore->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        leftScore->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(leftScore, 0, 1, 1, 1);

        leftTitle = new QLabel(ScorePanel);
        leftTitle->setObjectName(QString::fromUtf8("leftTitle"));
        leftTitle->setFont(font);
        leftTitle->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        leftTitle->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(leftTitle, 0, 0, 1, 1);

        meTitle = new QLabel(ScorePanel);
        meTitle->setObjectName(QString::fromUtf8("meTitle"));
        meTitle->setFont(font);
        meTitle->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        meTitle->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(meTitle, 2, 0, 1, 1);


        retranslateUi(ScorePanel);

        QMetaObject::connectSlotsByName(ScorePanel);
    } // setupUi

    void retranslateUi(QWidget *ScorePanel)
    {
        ScorePanel->setWindowTitle(QCoreApplication::translate("ScorePanel", "Form", nullptr));
        score3->setText(QCoreApplication::translate("ScorePanel", "\345\210\206", nullptr));
        score2->setText(QCoreApplication::translate("ScorePanel", "\345\210\206", nullptr));
        score1->setText(QCoreApplication::translate("ScorePanel", "\345\210\206", nullptr));
        meScore->setText(QString());
        rightScore->setText(QString());
        rightTitle->setText(QCoreApplication::translate("ScorePanel", "\345\217\263\344\276\247\346\234\272\345\231\250\344\272\272", nullptr));
        leftScore->setText(QString());
        leftTitle->setText(QCoreApplication::translate("ScorePanel", "\345\267\246\344\276\247\346\234\272\345\231\250\344\272\272", nullptr));
        meTitle->setText(QCoreApplication::translate("ScorePanel", "\346\210\221", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScorePanel: public Ui_ScorePanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCOREPANEL_H

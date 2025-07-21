#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QLineEdit>
#include "codec.h"

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);

    bool verityData(QLineEdit* edit);
    void onLogin();
    void onRegister();
    void onNetOk();
    void startConnect(Message* msg);

    //记住密码
    void saveUserInfo();
    //加载密码
    void loadUserInfo();

    ~login();

private:
    Ui::login *ui;
    bool m_isConnect=false;
    QByteArray Key="辛勤的蜜蜂永没有时间悲哀";
};

#endif // LOGIN_H

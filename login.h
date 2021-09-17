#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "component.h"
#include "dashboard.h"

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();
    dashboard *indexPage;

private slots:
    void on_pushButton_clicked();

private:
    Ui::login *ui;
};

class User{
public:
    string username;
    string password;
    bool isRoot;
    User(string username,string password,bool isRoot);
};

#endif // LOGIN_H

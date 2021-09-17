#include "login.h"
#include "ui_login.h"
#include<iostream>
#include "component.h"
#include "vector"
#include "QMessageBox"

using namespace std;

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    this->setWindowTitle("管理员登录");
}

login::~login()
{
    delete ui;
}

void login::on_pushButton_clicked()
{
    string username = ui->username->text().toStdString();
    string password = ui->password->text().toStdString();

    vector<User>adminList;
    adminList.push_back(User("root","root",true));
    adminList.push_back(User("admin","admin",false));
    adminList.push_back(User("stu","abcdef",false));
    for (auto obj : adminList)
    {
        if(obj.username==username&&obj.password==password){
            this->indexPage = new dashboard(obj.isRoot);
            this->indexPage->show();
            this->close();
            return;
        };
    }
    QMessageBox::about(NULL, "错误", "账号或密码错误，请重试！");
}

User::User(string username0,string password0,bool isRoot0):username(username0),password(password0),isRoot(isRoot0){};

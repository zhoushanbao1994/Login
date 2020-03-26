#include "register.h"
#include "ui_register.h"
//#include "login.h"

Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);

    setWindowTitle(tr("注册"));
}

Register::~Register()
{
    delete ui;
}

void Register::setParent(Login *dialog)
{
    if(dialog != NULL){
        pWidget = dialog;
    }
}


//注册函数
void Register::on_registerCheckButton_clicked()
{
    QString regUserName = ui->lineEditName->text();
    QString regPasswd = ui->lineEditPassword->text();
    QString regEmail = ui->lineEditEmail->text();


    if(regUserName.isEmpty() || regPasswd.isEmpty() || regEmail.isEmpty()){
        QMessageBox::information(this,tr("提示"),tr("请输入用户名、密码、邮箱！"));
        return;
    }

    if(App::sqlite->searchUserByName(regUserName)) {
        QMessageBox::warning(this,tr("警告"),tr("用户已存在！"));
        return;
    }
    if(App::sqlite->searchUserByEmail(regEmail)) {
        QMessageBox::warning(this,tr("警告"),tr("邮箱已存在！"));
        return;
    }
    if(!App::sqlite->addUser(regUserName, regPasswd, regEmail, 1)) {
        QMessageBox::information(this,tr("提示"),tr("注册失败！"));
        return;
    }

    QMessageBox::information(this,tr("提示"),tr("注册成功！"));
    this->close();
}

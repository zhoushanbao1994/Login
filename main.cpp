#include <QApplication>
#include <QtWidgets>
#include "login.h"
#include "systemtrayicon.h"
#include "exam.h"

#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());

    Login *login = new Login;
    login->show();

    Exam *e = new Exam(login);
    QObject::connect(login, &Login::signal_loginSuccessful,
                     e, &QMainWindow::show);
    
    return a.exec();
}

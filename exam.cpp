#include "exam.h"
#include "ui_exam.h"

#include <QtCore>

Exam::Exam(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Exam)
{
    ui->setupUi(this);

    // 设置窗口为模态窗口，屏蔽父窗口操作
    setWindowModality(Qt::ApplicationModal);

    init();
}

Exam::~Exam()
{
    delete ui;
}

void Exam::init()
{
    setWindowTitle(tr("在线考试"));
}

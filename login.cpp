#include "login.h"
#include "ui_login.h"

#include "exam.h"
#include "passwdedit.h"
#include "register.h"
#include "systemtrayicon.h"

#include <QMessageBox>
#include <QUrl>
#include <QDesktopServices>
#include <QDir>
#include <QDebug>
#include <QMenu>

float opacity1 = 0.0, opacity2 = 1.0;

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    // 设置任务栏托盘图标
    QStringList strList;
    strList << "QQ登陆" << "Login v1.0";
    QIcon icon(":/images/QQ.png");
    SystemTrayIcon *trayIcon = new SystemTrayIcon(strList, icon, this);
    connect(trayIcon, SIGNAL(signal_showWin()), this, SLOT(slot_trayIcon()));   // 关闭

    init();

    connect(this,SIGNAL(close()),this,SLOT(close()));

    //隐藏任务栏图标
    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::WindowStaysOnTopHint | Qt::Tool;
    setWindowFlags(flags);
}

Login::~Login()
{
    delete ui;
}

void Login::init()
{
    setWindowTitle(tr("登录"));

//    ui->btn_edit_pwd->setStyleSheet("background-color:transparent;");
//    ui->btn_regist->setStyleSheet("background-color:transparent;");
//    ui->btn_login->setStyleSheet("background-color:transparent;");

    m_Drag = false;

    timer1 = new QTimer;
    timer1->start(5);
    timer2 = new QTimer;
    connect(timer1, SIGNAL(timeout()), this, SLOT(slot_timer1()));
    connect(timer2, SIGNAL(timeout()), this, SLOT(slot_timer2()));

    configWindow();//UI界面设置  去边框，最小化，最大化button

    init_sql();//初始化界面密码，帐号的初值

    //init记住密码
    ui->checkBox_rPasswd->setChecked(true);
    ui->lineEdit_passwd->setEchoMode(QLineEdit::Password);
}

void Login::configWindow()
{

    // 填充背景图片
     QPalette palette;
     palette.setBrush(/*QPalette::Background*/this->backgroundRole(),
                      //QBrush(QPixmap(":/images/QQ1.png")));
                        QBrush(QPixmap(":/images/background.png")));
     this->setPalette(palette);


    //去掉窗口边框
    setWindowFlags(Qt::FramelessWindowHint);

    //设置top ,user img
    set_top_img(true, -1);//设置图片显示为随机显示
    set_user_img(true, -1);//设置user图片为随机显示
    qDebug() << "xxxxxxxxxxxxxxxxxxxxxxx" ;

    //设置UI的按钮button
    set_button();

}


void Login::init_sql()
{
    App::sqlite->tableInit();
    if(!App::sqlite->searchUserByName("admin")) {  // 如果初始管理员不存在
         App::sqlite->addUser("admin", "admin", "admin@qq.com", -1);    // 创建初始管理员用户
    }

    QStringList usernames;
    QStringList passwds;
    // 获取所有的用户密码
    App::sqlite->searchAllUser(usernames, passwds);

    // 填写登录用户，保存密码
    ui->cBox_account->addItems(usernames);
    m_allUserPasswd = passwds;

    // 设置默认用户、密码
    ui->cBox_account->setCurrentIndex(0);
    ui->lineEdit_passwd->setText(m_allUserPasswd.at(0));
}

// 设置顶部图片
void Login::set_top_img(bool isSandom, int index_img)
{
    //427 185
    int set_index_img = 1;
    if(isSandom == true)//随机显示topimg
    {

        QTime time_sand;
        time_sand= QTime::currentTime();//获取当前时间
        qsrand(time_sand.msec()+time_sand.second()*1000);

        //index_img = qrand()%5 ;//在0-4中选出随机数
        set_index_img = qrand()%5 + 1 ;//在1-5中选出随机数

    }
    else if(isSandom == false) //不随机显示，按index_img显示图片s
    {
        set_index_img = index_img;
    }

    QString top_img_path=":/images/top_img1.png";
    qDebug()<< "             [leo]" << top_img_path;
    QImage top_img;
    top_img_path = ":/images/top_img" + QString::number(set_index_img, 10)  + ".png";
    qDebug()<< "             [leo]" << top_img_path;
    top_img.load(top_img_path);
    QPixmap top_pic=QPixmap::fromImage(top_img.scaled(ui->label_top_img->width(),ui->label_top_img->height()));
    ui->label_top_img->setPixmap(top_pic);
    qDebug() << "             [leo]top_img width heigh:" << ui->label_top_img->width()
             << " " << ui->label_top_img->height();
}

// 设置按键样式
void Login::set_button()
{
    //构建最小化、关闭按钮,设置按钮，键盘ico
    minBtn = new QToolButton(this);
    closeBbtn = new QToolButton(this);
    setBtn = new QToolButton(this);
    keyBtn = new QToolButton(this);

//    //获取最小化、关闭按钮图标
//    QPixmap minPix  = style()->standardPixmap(QStyle::SP_TitleBarMinButton);
//    QPixmap closePix = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
//    //设置最小化、关闭按钮图标
//    minBtn->setIcon(minPix);
//    closeBbtn->setIcon(closePix);

    //获取界面的宽度
    int width = this->width();
    //设置最小化、关闭按钮在界面的位置
    minBtn->setGeometry(width-55,5,20,20);
    closeBbtn->setGeometry(width-25,5,20,20);
    setBtn->setGeometry(width-80,7,15,15);
    //设置键盘ico坐标
    int x = ui->lineEdit_passwd->x();
    int y = ui->lineEdit_passwd->y();
    int widthkey = ui->lineEdit_passwd->width();
    keyBtn->setGeometry(x+widthkey-20, y, 20, 20);

    qDebug() << "[leo]width:" << width ;
    qDebug() << "[leo]minBtn" << minBtn->geometry();
    qDebug() << "[leo]closeBbtn" << closeBbtn->geometry();

    //设置鼠标移至按钮上的提示信息
    minBtn->setToolTip(tr("最小化"));
    closeBbtn->setToolTip(tr("关闭"));
    setBtn->setToolTip(tr("设置"));
    keyBtn->setToolTip(tr("虚拟键盘"));

    //设置最小化、关闭按钮的样式图标
    minBtn->setIcon(QIcon(":/images/ico/mini.png"));
    minBtn->setStyleSheet("background-color:transparent;");
    closeBbtn->setIcon(QIcon(":/images/ico/close.png"));
    closeBbtn->setStyleSheet("background-color:transparent;");
    setBtn->setIcon(QIcon(":/images/ico/setting.png"));
    setBtn->setStyleSheet("background-color:transparent;");
    keyBtn->setIcon(QIcon(":/images/keyBoard.png"));
    keyBtn->setStyleSheet("background-color:transparent;");

    //关联最小化、关闭按钮的槽函数,键盘exe
    connect(minBtn, SIGNAL(clicked()), this, SLOT(slot_minWindow()));
    connect(closeBbtn, SIGNAL(clicked()), this, SLOT(slot_closeWindow()));
    connect(keyBtn, SIGNAL(clicked()), this, SLOT(slot_getKeyBoard()));
    connect(setBtn, SIGNAL(clicked()), this, SLOT(slot_setLanguage()));

    //添加在线状态toolButton
    status_tBtn = new QToolButton(this);
    int _x = ui->label_user_img->x();
    int _y = ui->label_user_img->y();
    int _width = ui->label_user_img->width();
    int _heigh = ui->label_user_img->height();
    status_tBtn->setGeometry(_x+_width-20, _y+_heigh-20, 20, 20);
    status_tBtn->setIcon(QIcon(":/images/ico/setting.png"));
    status_tBtn->setToolTip(tr("在线状态"));
    connect(status_tBtn, SIGNAL(clicked()), this, SLOT(slot_setStatus()));

    create_menuLanguage();      //创建语言菜单
}

void Login::set_user_img(bool isSandom, int index_img)
{
    //40,182 85 85
    int set_index_img = 1;
    if(isSandom == true)//随机显示userimg
    {

        QTime time_sand;
        time_sand= QTime::currentTime();//获取当前时间
        qsrand(time_sand.msec()+time_sand.second()*1000);
        set_index_img = qrand()%5 + 1 ;//在1-5中选出随机数

    }
    if(isSandom == false) //不随机显示，按index_img显示图片s
    {
        set_index_img = index_img;
    }

    QString user_img_path=":/images/ico/user1.png";
    qDebug()<< "             [leo]user" << user_img_path;
    QImage user_img;
    user_img_path = ":/images/ico/user" + QString::number(set_index_img, 10)  + ".png";
    qDebug()<< "             [leo]user" << user_img_path;
    user_img.load(user_img_path);
    QPixmap img_pic=QPixmap::fromImage(user_img.scaled(ui->label_user_img->width(),
                                                       ui->label_user_img->height()));
                                       ui->label_user_img->setPixmap(img_pic);
    qDebug() << "             [leo]user_img width heigh:" << ui->label_user_img->width()
             << " " << ui->label_user_img->height();
}

void Login::create_menuLanguage()
{
    //语言
    act_chinese = new QAction(tr("Chinese"), this);
    act_english = new QAction(tr("English"), this);
    menu1 = new QMenu;
    menu1->addAction(act_chinese);
    menu1->addAction(act_english);

    //在线状态
    act0 = new QAction(tr("在线"), this);
    act1 = new QAction(tr("隐身"), this);
    act2 = new QAction(tr("离线"), this);
    act3 = new QAction(tr("忙碌"), this);

    actGrp = new QActionGroup(this);
    actGrp->addAction(act0);
    actGrp->addAction(act1);
    actGrp->addAction(act2);
    actGrp->addAction(act3);
    connect(actGrp, SIGNAL(triggered(QAction*)), this, SLOT(slot_actGrp(QAction*)));

    menu2 = new QMenu;
    menu2->addAction(act0);
    menu2->addAction(act1);
    menu2->addAction(act2);
    menu2->addAction(act3);
}


void Login::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        m_Drag = true;
        m_point = e->globalPos() - this->pos();
        e->accept();
        qDebug()<<"leo";
    }
}

void Login::mouseMoveEvent(QMouseEvent *e)
{
    if (m_Drag && (e->buttons() && Qt::LeftButton)) {
        move(e->globalPos() - m_point);
        e->accept();
        qDebug()<<"leomove";
    }
}

void Login::mouseReleaseEvent(QMouseEvent *e)
{
    m_Drag = false;
}

// 登录按键
void Login::on_btn_login_clicked()
{
    // 获取界面上输入的用户密码
    QString userName = ui->cBox_account->currentText();
    QString passwd = ui->lineEdit_passwd->text();

    if(userName.isEmpty() || passwd.isEmpty()) {
        QMessageBox::warning(this,tr("警告"),tr("请输入用户名和密码！"));
    }
    else {
        if(App::sqlite->searchUser(userName, passwd)) {
            //用户名和密码匹配
            Exam *e = new Exam;
            e->show();
            emit close();
        }
        else {
            QMessageBox::information(this,tr("提示"),tr("用户不存在或密码错误！"));
        }
    }
}

//注册button
void Login::on_btn_regist_clicked()
{
    Register *r = new Register(this);   // 注册窗口
    r->setParent(this);      //设置父对象
    r->exec();
}

//修改密码button
void Login::on_btn_edit_pwd_clicked()
{
    QString username = ui->cBox_account->currentText();
    QString passwd = ui->lineEdit_passwd->text();

    if(username.isEmpty() || passwd.isEmpty()){
        QMessageBox::information(this,tr("提示"),tr("请输入原用户名和密码！"));
        return;
    }
    if(!App::sqlite->searchUser(username, passwd)) {
        QMessageBox::information(this,tr("提示"),tr("原用户名或密码错误！"));
        return;
    }

    else
    {
        passwdEdit *passwdEditDlg = new passwdEdit(username, passwd, this);
        passwdEditDlg->setLogin(this);
        passwdEditDlg->exec();
    }
}

void Login::slot_minWindow()
{
    this->showMinimized();
}

void Login::slot_closeWindow()
{
    timer2->start(5);
}

// 托盘图标关闭事件的槽函数
void Login::slot_trayIcon()
{
    timer1->start(5);
    qDebug() << "trayIcon doubleClicked!";
}

void Login::slot_getKeyBoard()
{
    qDebug() << "key!";

//    QString m_path("D:/login/images/osk.exe");
//    int ret = curPath.compare(m_path);
//    qDebug() << "curPath:" << curPath << "ret:" << ret;
//    QString path;
//    if (ret != 0){
//        QStringList pathList = curPath.split("/");
//        path = pathList.at(0);
//        path.append("/login/images/osk.exe");
//    }else{
//        path = m_path;
//    }
    QString curPath = QApplication::applicationDirPath();
    curPath.append("/osk.exe");
    qDebug() << "curPath:" << curPath;

    QDesktopServices::openUrl(QUrl(curPath, QUrl::TolerantMode));
}

void Login::slot_setLanguage()
{
    menu1->exec(QCursor::pos());
}

void Login::slot_setStatus()
{
    menu2->exec(QCursor::pos());
}

void Login::slot_actGrp(QAction *act)
{
    if (act == act0) {
        status_tBtn->setToolTip("在线");
        qDebug() << "act0";
    } else if (act == act1) {
        status_tBtn->setToolTip("隐身");
        qDebug() << "act1";
    } else if (act == act2) {
        status_tBtn->setToolTip("离线");
        qDebug() << "act2";
    } else if (act == act3) {
        status_tBtn->setToolTip("忙碌");
        qDebug() << "act3";
    }
}

void Login::slot_timer1()
{
    if (opacity1 >= 1.0) {
        timer1->stop();
    }else{
        opacity1 += 0.01;
    }
    setWindowOpacity(opacity1);//设置窗口透明度
}

void Login::slot_timer2()
{
    if (opacity2 <= 0.0) {
        timer2->stop();

        this->close();
    }else{
        opacity2 -= 0.01;
    }
    setWindowOpacity(opacity2);//设置窗口透明度
}

void Login::on_cBox_account_activated(int index)
{
    ui->lineEdit_passwd->setText(m_allUserPasswd.at(index));
    qDebug() << "change cBox:" << ui->cBox_account->currentText()
             << m_allUserPasswd.at(index);
}


//下拉框选里面的项时，会切换top_img的图片和头像图片
void Login::on_cBox_account_currentIndexChanged(int index)
{
   set_top_img(true,index);
   set_user_img(true,index);
}


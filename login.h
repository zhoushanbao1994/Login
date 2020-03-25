#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMouseEvent>
#include <QTimer>
#include <QToolButton>
#include <QMenu>
#include <QAction>
#include <QActionGroup>

#include "sqlite.h"
#include "app.h"

struct UserInfoStu{
    QString userName;
    QString passwd;
    QString email;
};

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT
    
public:
    explicit Login(QWidget *parent = 0);
    ~Login();

    void init();
    void configWindow();
    void init_sql();
    void set_button();//设置UI上的按钮
    void set_top_img(bool isSandom, int index_img);//isSandom is true,set img show by sandom
    void set_user_img(bool isSandom, int index_img);//设置UI上用户头像

    void create_menuLanguage();         //设置语言菜单

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

signals:
    void close();
    
private slots:
    void on_btn_login_clicked();    // 登录按键
    void on_btn_regist_clicked();   // 注册按键

    void slot_minWindow();          // 最小化
    void slot_closeWindow();        // 关闭

    void slot_trayIcon();           // 托盘图标关闭事件的槽函数

    void slot_getKeyBoard();

    void slot_setLanguage();        //设置语言

    void slot_setStatus();          //设置在线状态

    void slot_actGrp(QAction* act);

    void on_cBox_account_activated(int index);

    void on_btn_edit_pwd_clicked();

    void on_cBox_account_currentIndexChanged(int index);

    void slot_timer1();
    void slot_timer2();

private:
    Ui::Login *ui;

    bool m_Drag;
    QPoint m_point;

    QTimer *timer1, *timer2;

    QStringList m_allUserPasswd;         //用户密码

    QToolButton *minBtn;        // 最小化
    QToolButton *closeBbtn;     // 关闭
    QToolButton *setBtn;
    QToolButton *keyBtn;        // 键盘按键
    QToolButton *status_tBtn;       //在线状态

    QMenu *menu1;                   //语言菜单
    QAction *act_chinese;
    QAction *act_english;

    QMenu *menu2;                   //在线状态
    QAction *act0;                  //在线
    QAction *act1;                  //隐身
    QAction *act2;                  //离线
    QAction *act3;                  //忙碌
    QActionGroup *actGrp;


};

#endif // LOGIN_H

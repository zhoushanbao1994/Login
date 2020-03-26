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
    void on_btn_edit_pwd_clicked(); // 修改密码按键

    void slot_minWindow();          // 最小化
    void slot_closeWindow();        // 关闭
    void slot_setLanguage();        // 设置语言
    void slot_getKeyBoard();        // 虚拟键盘按键
    void slot_setStatus();          // 设置在线状态

    void slot_actGrp(QAction* act); // 头像上的在线状态按键（选择了菜单栏中的具体哪个）

    void slot_trayIcon();           // 托盘图标关闭事件的槽函数

    // 用户名下来框
    void on_cBox_account_activated(int index);
    void on_cBox_account_currentIndexChanged(int index);

    void slot_timer1();  // 用于设置窗口打开时窗口的透明度 0.00~1.00
    void slot_timer2();  // 用于设置窗口关闭时窗口的透明度 1.00~0.00，关闭窗口

private:
    Ui::Login *ui;

    // 与窗口拖动有关
    bool m_Drag;
    QPoint m_point;     // 左键点击窗口空白区域时记录点击坐标

    QTimer *timer1; // 用于设置窗口打开时窗口的透明度 0.00~1.00
    QTimer *timer2; // 用于设置窗口关闭时窗口的透明度 1.00~0.00，关闭窗口

    float opacity1 = 0.0;   // 调解透明的过程中使用的数值
    float opacity2 = 1.0;


    QStringList m_allUserPasswd;         //用户密码

    QToolButton *minBtn;        // 最小化
    QToolButton *closeBbtn;     // 关闭
    QToolButton *setBtn;        // 设置按键
    QToolButton *keyBtn;        // 键盘按键
    QToolButton *status_tBtn;   // 在线状态

    QMenu *menu_language;   // 语言菜单
    QAction *act_chinese;   // 语言菜单-中文
    QAction *act_english;   // 语言菜单-英文

    QMenu *menu_onlinestatus;   // 头像上的在线状态按键
    QAction *act_online;        // 头像上的在线状态按键-在线
    QAction *act_stealth;       // 头像上的在线状态按键-隐身
    QAction *act_offline;       // 头像上的在线状态按键-离线
    QAction *act_busy;          // 头像上的在线状态按键-忙碌
    QActionGroup *actGrp;


};

#endif // LOGIN_H

#ifndef SQLITE_H
#define SQLITE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>

#include <QString>
#include <QDebug>

//// 表名
//#define USERINFO    ( QString("userInfo")    )       // 表名
//
//// 数据库表中的字段
//#define ID          ( "id"          )       // id
//#define USERNAME    ( "username"    )       // 用户名
//#define PASSWD      ( "passwd"      )       // 密码
//#define EMAIL       ( "email"       )       // 邮箱
//#define RANK        ( "rank"        )       // 权限


typedef struct StruUserInfo{
    int id;
    QString userName;
    QString passwd;
    QString email;
    int rank;
}StruUserInfo;

class SQLite
{
public:
    SQLite(const QString dbname);
    void tableInit();

    // 增加用户
    bool addUser(StruUserInfo unsrInfo);
    bool addUser(QString username, QString passwd, QString email, int rank);

    // 查找用户
    bool searchUser(QString username, QString passwd, StruUserInfo *userInfo = nullptr);
    bool searchUserByName(QString username, StruUserInfo *userInfo = nullptr);
    bool searchUserByEmail(QString email, StruUserInfo *userInfo = nullptr);
    bool searchAllUser(QStringList &username, QStringList &passwd);

    // 更新用户信息
    bool updateUserPasswd(QString username, QString oldpasswd, QString newpasswd);

private:
    QSqlDatabase m_db;      // 数据库
    QSqlQuery *m_check;     // 执行SQL语句

    QString md5(QString s);     // MD5 加密
};

#endif // SQLITE_H

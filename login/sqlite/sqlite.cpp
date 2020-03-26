#include "sqlite.h"
#include <QCryptographicHash>

SQLite::SQLite(const QString dbname)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");// 数据库类型
    m_db.setDatabaseName(dbname);               // 数据库名

    m_check = new QSqlQuery(m_db);              // 初始化QSqlQuery，执行SQL语句关联的数据库

    if (!m_db.open()) {
        qDebug() << "database open fail!";
    }
    else {
        qDebug() << "database open success!";
    }
}


QString SQLite::md5(QString s)
{
    QString md5;
    QByteArray bb;

    bb = QCryptographicHash::hash ( s.toLatin1(), QCryptographicHash::Md5 );
    md5.append(bb.toHex());

    return md5;
}

void SQLite::tableInit()
{
    // 如果表不存在，则创建表 userInfo
    // id           整数 主键   自动递增
    // username     文本 不为空 唯一
    // passwd       文本 不为空
    // reank        整数 默认值1
    // email        文本 不为空 唯一
    QString sql_create_table = "CREATE TABLE IF NOT EXISTS userInfo \
            ( \
            id INTEGER PRIMARY KEY AUTOINCREMENT, \
            username TEXT NOT NULL UNIQUE, \
            passwd TEXT NOT NULL, \
            rank INTEGER DEFAULT (1), \
            email TEXT NOT NULL UNIQUE\
            )";
    m_check->prepare(sql_create_table);
    if (!m_check->exec()) {  // 执行SQL语句。执行失败
        qDebug() << "creater table error";
    }
    else {
        qDebug() << "creater table success";
    }
}


bool SQLite::addUser(StruUserInfo userInfo)
{
    return addUser(userInfo.userName, userInfo.passwd, userInfo.email, userInfo.rank);
}

bool SQLite::addUser(QString username, QString passwd, QString email, int rank)
{
    m_check->prepare("INSERT INTO userInfo(username, passwd, email, rank)"\
                       "VALUES(:un, :pw, :em, :rank)");
    m_check->bindValue(":un", username);
    m_check->bindValue(":pw", md5(passwd));
    m_check->bindValue(":em", email);
    m_check->bindValue(":rank", rank);

    // 执行成功，
    if(m_check->exec()) {
        qDebug() << "add user success";
        return true;
    }
    qDebug() << "add user error";
    return false;
}


// 查找用户
bool SQLite::searchUser(QString username, QString passwd, StruUserInfo *userInfo)
{
    m_check->prepare("SELECT * FROM userInfo WHERE username = (:un) AND passwd = (:pw)");
    m_check->bindValue(":un", username);
    m_check->bindValue(":pw", md5(passwd));

    if (!m_check->exec()) {  // 执行SQL语句。执行失败
        return false;  // 返回结果
    }
    if (!m_check->next()) {  // 执行结果中没有数据
        return false;
    }

    if(userInfo != nullptr) {
        // 获取查询结果
        int idUserId    = m_check->record().indexOf("id");
        int idUsername  = m_check->record().indexOf("username");
        int idPasswd    = m_check->record().indexOf("passwd");
        int idEmail     = m_check->record().indexOf("email");
        int idRank      = m_check->record().indexOf("rank");

        userInfo->id         = m_check->value(idUserId).toInt();
        userInfo->userName   = m_check->value(idUsername).toString();
        userInfo->passwd     = m_check->value(idPasswd).toString();
        userInfo->email      = m_check->value(idEmail).toString();
        userInfo->rank       = m_check->value(idRank).toInt();
    }

    return true;
}

bool SQLite::searchUserByName(QString username, StruUserInfo *userInfo)
{
    m_check->prepare("SELECT * FROM userInfo WHERE username = (:un)");
    m_check->bindValue(":un", username);

    if (!m_check->exec()) {  // 执行SQL语句。执行失败
        return false;  // 返回结果
    }
    if (!m_check->next()) {  // 执行结果中没有数据
        return false;
    }

    if(userInfo != nullptr) {
        // 获取查询结果
        int idUserId    = m_check->record().indexOf("id");
        int idUsername  = m_check->record().indexOf("username");
        int idPasswd    = m_check->record().indexOf("passwd");
        int idEmail     = m_check->record().indexOf("email");
        int idRank      = m_check->record().indexOf("rank");

        userInfo->id         = m_check->value(idUserId).toInt();
        userInfo->userName   = m_check->value(idUsername).toString();
        userInfo->passwd     = m_check->value(idPasswd).toString();
        userInfo->email      = m_check->value(idEmail).toString();
        userInfo->rank       = m_check->value(idRank).toInt();
    }

    return true;
}

bool SQLite::searchUserByEmail(QString email, StruUserInfo *userInfo)
{
    m_check->prepare("SELECT * FROM userInfo WHERE email = (:em)");
    m_check->bindValue(":em", email);

    if (!m_check->exec()) {  // 执行SQL语句。执行失败
        return false;  // 返回结果
    }
    if (!m_check->next()) {  // 执行结果中没有数据
        return false;
    }

    if(userInfo != nullptr) {
        // 获取查询结果
        int idUserId    = m_check->record().indexOf("id");
        int idUsername  = m_check->record().indexOf("username");
        int idPasswd    = m_check->record().indexOf("passwd");
        int idEmail     = m_check->record().indexOf("email");
        int idRank      = m_check->record().indexOf("rank");

        userInfo->id         = m_check->value(idUserId).toInt();
        userInfo->userName   = m_check->value(idUsername).toString();
        userInfo->passwd     = m_check->value(idPasswd).toString();
        userInfo->email      = m_check->value(idEmail).toString();
        userInfo->rank       = m_check->value(idRank).toInt();
    }

    return true;
}

bool SQLite::searchAllUser(QStringList &username, QStringList &passwd)
{
    m_check->prepare("SELECT * FROM userInfo");

    if (!m_check->exec()) {  // 执行SQL语句。执行失败
        return false;  // 返回结果
    }
    int flag = 0;
    while(m_check->next()) {  // 执行结果中有数据
        // 获取查询结果
        int idUsername  = m_check->record().indexOf("username");
        int idPasswd    = m_check->record().indexOf("passwd");

        username.append(m_check->value(idUsername).toString());
        passwd.append(m_check->value(idPasswd).toString());

        flag++;
    }

    if(flag == 0) {
        return false;
    }

    return true;
}

// 更新用户信息
bool SQLite::updateUserPasswd(QString username, QString oldpasswd, QString newpasswd)
{
    m_check->prepare("UPDATE userInfo SET passwd=(:npw) WHERE username=(:un) AND passwd=(:opw)");
    m_check->bindValue(":un", username);
    m_check->bindValue(":opw", md5(oldpasswd));
    m_check->bindValue(":npw", md5(newpasswd));

    if(m_check->exec()) { // 执行成功
        return true;
    }
    return false;
}




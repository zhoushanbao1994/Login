#ifndef APP_H
#define APP_H

#include "sqlite.h"

class App
{
public:
    App();

    static SQLite *sqlite;    // 数据库相关
};

#endif // APP_H

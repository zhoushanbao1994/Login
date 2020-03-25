#include "app.h"

#include <QCoreApplication>


SQLite *App::sqlite = new SQLite(QCoreApplication::applicationDirPath()+"/sqlite.db");

App::App()
{

}

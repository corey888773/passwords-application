#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "databasemanager.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "KeyHandler_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;

    struct connection_details *mysqlD = new connection_details;
    mysqlD->server = "localhost";
    mysqlD->user = "piotrek";
    mysqlD->password = "Piotrekrank12";
    mysqlD->database = "Passwords";

    struct user_data *userData = new user_data;
    userData->site_name = "test3";
    userData->email = "test3@mail.com";
    userData->username = "test3";
    userData->passowrd = "Password3";

    auto *data = new DataBaseManager(mysqlD);
    data->select_all();
    data->showResult();

    delete data;
    delete mysqlD;
    delete userData;


    w.show();
    return a.exec();
}

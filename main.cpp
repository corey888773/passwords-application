#include "mainwindow.h"
#include "databasemanager.h"

#include <time.h>
#include <QApplication>
#include <QLocale>
#include <QTranslator>




int main(int argc, char *argv[])
{

    srand(time(NULL));

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


//    data->select_all();
//    data->showResult();









    w.show();
    return a.exec();
}

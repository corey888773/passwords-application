#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QClipboard>
#include "databasemanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    DataBaseManager *data;
    struct connection_details *mysqlD;
    struct user_data *userData;
    QClipboard *clipBoard;
    bool dataChange;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    char *to_charPtr(QString text);
    void reset_list();

private slots:
    void set_current_data();
    void change_data();
    void copy_password_to_clipboard();
    void delete_user();
    void add_user();
};
#endif // MAINWINDOW_H

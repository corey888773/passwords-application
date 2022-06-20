#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QClipboard>
#include "databasemanager.h"
#include "passwordcoder.h"

namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    enum lock{
      locked = 0,
      unlocked = 1
    };

    Q_OBJECT
    DataBaseManager *data;
    PasswordCoder *pass;
    struct connection_details *mysqlD;
    struct user_data *userData;
    QClipboard *clipBoard;
    lock dataChange;
    lock lockPassword;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    char *to_charPtr(QString text);
    void reset_list();
    void get_user();
    void clear_add_user_fields();

private slots:
    void set_current_data();
    void change_data();
    void copy_password_to_clipboard();
    void delete_user();
    void add_user();
    void lock_password();
};
#endif // MAINWINDOW_H

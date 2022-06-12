#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mysqlD = new connection_details;
    mysqlD->server = "localhost";
    mysqlD->user = "piotrek";
    mysqlD->password = "Piotrekrank12";
    mysqlD->database = "Passwords";

    userData = new user_data;
    userData->site_name = "test3";
    userData->email = "test3@mail.com";
    userData->username = "test3";
    userData->password = "Password3";


    data = new DataBaseManager(mysqlD);
    clipBoard = QApplication::clipboard();
    dataChange = false;

    reset_list();
    set_current_data();
    connect(ui->sitesList, SIGNAL(currentIndexChanged(int)), this, SLOT(set_current_data()));
    connect(ui->getPasswordButton, SIGNAL(released()), this, SLOT(copy_password_to_clipboard()));
    connect(ui->changeDataButton, SIGNAL(released()), this, SLOT(change_data()));
    connect(ui->deleteDataButton, SIGNAL(released()), this, SLOT(delete_user()));
    connect(ui->saveButton, SIGNAL(released()), this, SLOT(add_user()));

}

char* MainWindow::to_charPtr(QString text){
//    QByteArray ba = text.toUtf8();
//    char *c_str2 = ba.data();
//    return c_str2;
      auto str = text.toStdString();
      char *cstr = new char[str.length() + 1];
      strcpy(cstr, str.c_str());
      return cstr;

}

void MainWindow::reset_list(){
    ui->sitesList->clear();
    for( auto i = 0; i < data->sites_names.size(); i++ )
    {
         if(ui->sitesList)
         ui->sitesList->addItem(data->sites_names[i]);
    }
}

void MainWindow::set_current_data(){
    if(ui->sitesList->currentText() == "")
        return;
    auto present_user = data->gather_info(to_charPtr(ui->sitesList->currentText()));

    ui->username->setText(QString::fromUtf8(present_user.username));
    ui->emailAddress->setText(QString::fromUtf8(present_user.email));
    ui->password->setText(QString::fromUtf8(present_user.password));
}

void MainWindow::copy_password_to_clipboard(){
    clipBoard->setText(ui->password->text());
}

void MainWindow::change_data(){
    switch(dataChange){
    case true:
        ui->username->setReadOnly(true);
        ui->emailAddress->setReadOnly(true);
        ui->password->setReadOnly(true);
        dataChange = false;

        ui->changeDataButton->setText("change Data");

        break;
    case false:
        ui->username->setReadOnly(false);
        ui->emailAddress->setReadOnly(false);
        ui->password->setReadOnly(false);
        dataChange = true;

        ui->changeDataButton->setText("save");

        break;
    }
    std::cout<<"data_change" << std::endl;
}

void MainWindow::delete_user(){
    data->delete_data(to_charPtr(ui->sitesList->currentText()), to_charPtr((ui->username->text())));
    ui->sitesList->removeItem(ui->sitesList->currentIndex());
    ui->sitesList->setCurrentIndex(0);
}
void MainWindow::add_user(){
    userData->site_name = to_charPtr(ui->NewSitename->text());
    userData->email = to_charPtr(ui->NewEmailAddress->text());
    userData->username = to_charPtr(ui->NewUsername->text());
    userData->password = to_charPtr(ui->NewPassword->text());

    std::cout<< userData->site_name << std::endl;
    std::cout<< userData->email << std::endl;
    std::cout<< userData->username << std::endl;
    std::cout<< userData->password << std::endl;

    data->new_data(userData);
    reset_list();
}

MainWindow::~MainWindow()
{

    delete data;
    delete mysqlD;
    delete userData;
    delete ui;

}


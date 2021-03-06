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
    pass = new PasswordCoder(3);
    data = new DataBaseManager(mysqlD);
    clipBoard = QApplication::clipboard();


    dataChange = locked;
    lockPassword = unlocked;

    reset_list();
    set_current_data();
    connect(ui->sitesList, SIGNAL(currentIndexChanged(int)), this, SLOT(set_current_data()));
    connect(ui->getPasswordButton, SIGNAL(released()), this, SLOT(copy_password_to_clipboard()));
    connect(ui->changeDataButton, SIGNAL(released()), this, SLOT(change_data()));
    connect(ui->deleteDataButton, SIGNAL(released()), this, SLOT(delete_user()));
    connect(ui->saveButton, SIGNAL(released()), this, SLOT(add_user()));
    connect(ui->checkRandomPassword, SIGNAL(stateChanged(int)), this, SLOT(lock_password()));

}

char* MainWindow::to_charPtr(QString text){
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
    {
        ui->username->clear();
        ui->emailAddress->clear();
        ui->password->clear();
        return;
    }
    auto present_user = data->gather_info(to_charPtr(ui->sitesList->currentText()));

    ui->username->setText(QString::fromUtf8(present_user.username));
    ui->emailAddress->setText(QString::fromUtf8(present_user.email));
    ui->password->setText(QString::fromUtf8(present_user.password));

}

void MainWindow::copy_password_to_clipboard(){
    clipBoard->setText(pass->decode(ui->password->text()));
}

void MainWindow::change_data(){
    auto currIndex = ui->sitesList->currentIndex();
    switch(dataChange){
    case unlocked:
        userData->site_name = to_charPtr(ui->sitesList->currentText());
        userData->username = to_charPtr(ui->username->text());
        userData->email = to_charPtr(ui->emailAddress->text());
        userData->password = to_charPtr(ui->password->text());
        data->change_data(userData);

        ui->username->setReadOnly(true);
        ui->emailAddress->setReadOnly(true);
        ui->password->setReadOnly(true);
        dataChange = locked;

        ui->changeDataButton->setText("change Data");

        break;
    case locked:
        ui->username->setReadOnly(false);
        ui->emailAddress->setReadOnly(false);
        ui->password->setReadOnly(false);
        dataChange = unlocked;

        ui->changeDataButton->setText("save");

        break;
    }
    ui->sitesList->setCurrentIndex(currIndex);
}

void MainWindow::delete_user(){
    data->delete_data(to_charPtr(ui->sitesList->currentText()), to_charPtr((ui->username->text())));
    ui->sitesList->removeItem(ui->sitesList->currentIndex());
    ui->sitesList->setCurrentIndex(0);
}
void MainWindow::add_user(){
    get_user();

    std::cout<< userData->site_name << std::endl;
    std::cout<< userData->email << std::endl;
    std::cout<< userData->username << std::endl;
    std::cout<< userData->password << std::endl;

    data->new_data(userData);
    clear_add_user_fields();
    reset_list();
}

void MainWindow::lock_password(){
    switch(lockPassword){
        case unlocked:
            ui->NewPassword->setReadOnly(true);
            ui->NewPassword->clear();
            ui->NewPassword->setPlaceholderText("RANDOM");
            lockPassword = locked;
            break;
        case locked:
            ui->NewPassword->setReadOnly(false);
            ui->NewPassword->setPlaceholderText("password");
            lockPassword = unlocked;
            break;
    }
}

void MainWindow::get_user(){
    delete this->userData;
    this->userData = new user_data;
    userData->site_name = to_charPtr(ui->NewSitename->text());
    userData->email = to_charPtr(ui->NewEmailAddress->text());
    userData->username = to_charPtr(ui->NewUsername->text());

    switch(lockPassword){
        case locked:
            userData->password = pass->encode(pass->generate_password());
            break;
        case unlocked:
            userData->password = pass->encode(ui->NewPassword->text());
            break;
    }

}

void MainWindow::clear_add_user_fields(){
    ui->NewEmailAddress->clear();
    ui->NewUsername->clear();
    ui->NewSitename->clear();
    ui->NewPassword->clear();
}

MainWindow::~MainWindow()
{
    delete data;
    delete mysqlD;
    delete userData;
    delete pass;
    delete ui;

}


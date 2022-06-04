//
// Created by piotrek on 03.06.22.
//
#include <iostream>
#include "databasemanager.h"
#include <cstring>
#include <string>
#include <algorithm>

DataBaseManager::DataBaseManager(struct connection_details *c_details)  : connection_details(c_details) {
    connection = connection_setup();
    result = nullptr;
    sites_to_vector();
};

MYSQL *DataBaseManager::connection_setup() {
    MYSQL *connection = mysql_init(nullptr);

    if(!mysql_real_connect(connection, connection_details->server, connection_details->user,
                           connection_details->password, connection_details->database, 0, nullptr, 0)){
        std::cout << "Connection Error: " << mysql_error(connection) << std::endl;
        exit(1);
    }
    return connection;
}

MYSQL_RES *DataBaseManager::execute_query(const char *sql_query) {
    connection = this->connection_setup();
    if(mysql_query(connection, sql_query)){
        std::cout << "MySQL query Error: " << mysql_error(connection) << std::endl;
        exit(1);
    }
    return mysql_use_result(connection);
}

void DataBaseManager::select_row(char *id) {
    id = add_quote(id);
    const char* base = "select * from login_data where id = ";
    char* temp = new char[strlen(base) + strlen(id)];
    strcpy(temp, base);
    strcpy(temp + strlen(base), id);
    result = this->execute_query(temp);
}

void DataBaseManager::showResult() {
    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)) != nullptr){
        std::cout << row[0] << "/ " << row[1] << "/ "<< row[2] << "/ "<< row[3] << "/ " << row[4] << "/ " << std::endl;
    }
}

DataBaseManager::~DataBaseManager() {
    mysql_free_result(result);
    mysql_close(connection);
}

char *DataBaseManager::add_quote(const char *text) {
    char* temp = new char[strlen(text)+2];
    auto quote = "\'";
    strcpy(temp, quote);
    strcpy(temp+1, text);
    strcpy(temp + strlen(text) + 1,quote);

    return temp;

}

void DataBaseManager::new_data(struct user_data *userData) {

    auto site_name = add_quote(userData->site_name);
    auto email = add_quote(userData->email);
    auto username = add_quote(userData->username);
    auto password = add_quote(userData->passowrd);
    auto base = "INSERT INTO login_data (site_name, email, user_name, user_password) VALUES (";

    auto i = 0;
    char *query = new char[strlen(site_name) + strlen(email) + strlen(username) + strlen(password) + strlen(base) + 3];
    strcpy(query + i, base);
    i += strlen(base);
    strcpy(query + i, site_name);
    i += strlen(site_name);
    strcpy(query + i, ",");
    i += 1;
    strcpy(query + i, email);
    i += strlen(email);
    strcpy(query + i, ",");
    i += 1;
    strcpy(query + i, username);
    i += strlen(username);
    strcpy(query + i, ",");
    i += 1;
    strcpy(query + i, password);
    i += strlen(password);
    strcpy(query + i, ")");

    result = execute_query(query);
    sites_to_vector();
}

void DataBaseManager::delete_data(char *id) {
    id = add_quote(id);
    const char* base = "delete from login_data where id = ";
    char* temp = new char[strlen(base) + strlen(id)];
    strcpy(temp, base);
    strcpy(temp + strlen(base), id);

    result = this->execute_query(temp);
    sites_to_vector();
}

void DataBaseManager::select_all() {
    auto temp = "select * from login_data";
    result = this->execute_query(temp);
}

void DataBaseManager::sites_to_vector(){
    sites_names.clear();
    select_all();
    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)) != nullptr) {
        if (std::find(sites_names.begin(), sites_names.end(), row[1]) == sites_names.end()) {
            char *temp = new char[strlen(row[1])];
            strcpy(temp, row[1]);

            sites_names.push_back(temp);
        }
    }
}

void DataBaseManager::show_sites() {
    for(auto & sites_name : sites_names){
        std::cout<< sites_name << "\t";
    }
    std::cout << "\n";

}

user_data &DataBaseManager::gather_info(const char *site_name) {
    auto *res = new user_data;

    site_name = add_quote(site_name);
    const char* base = "select * from login_data where site_name = ";
    char* temp = new char[strlen(base) + strlen(site_name)];
    strcpy(temp, base);
    strcpy(temp + strlen(base), site_name);
    result = this->execute_query(temp);

    auto row = mysql_fetch_row(result);

    res->site_name = row[1];
    res->email = row[2];
    res->username = row[3];
    res->passowrd = row[4];

    return *res;
}



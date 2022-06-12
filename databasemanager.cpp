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
    std::vector<char *> memory_to_clear;

    id = add_quote(id);
    const char* base = "select * from login_data where id =";
    char* temp = new char[strlen(base) + strlen(id)];
    strncpy(temp, base, strlen(base));
    strncpy(temp + strlen(base), id,strlen(id));
    result = this->execute_query(temp);

    memory_to_clear.push_back(temp);
    dump(memory_to_clear);
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
    strncpy(temp, quote, 1);
    strncpy(temp+1, text, strlen(text));
    strncpy(temp + strlen(text) + 1,quote, 1);

    return temp;


}

void DataBaseManager::new_data(struct user_data *userData) {
    std::vector<char*> memory_to_clear;

    auto site_name = add_quote(userData->site_name);
    auto email = add_quote(userData->email);
    auto username = add_quote(userData->username);
    auto password = add_quote(userData->password);
    auto base = "INSERT INTO login_data (site_name, email, user_name, user_password) VALUES (";

    memory_to_clear.push_back(site_name);
    memory_to_clear.push_back(email);
    memory_to_clear.push_back(username);
    memory_to_clear.push_back(password);

    size_t i = 0;
    char *query = new char[strlen(site_name) + strlen(email) + strlen(username) + strlen(password) + strlen(base) + 4];
    strncpy(query + i, base, strlen(base));
    i += strlen(base);
    strncpy(query + i, site_name, strlen(site_name));
    i += strlen(site_name);
    strncpy(query + i, ",",1);
    i += 1;
    strncpy(query + i, email, strlen(email));
    i += strlen(email);
    strncpy(query + i, ",",1);
    i += 1;
    strncpy(query + i, username, strlen(username));
    i += strlen(username);
    strncpy(query + i, ",",1);
    i += 1;
    strncpy(query + i, password, strlen(password));
    i += strlen(password);
    strcpy(query + i, ")");

    result = execute_query(query);
    sites_to_vector();
    dump(memory_to_clear);
}

//void DataBaseManager::delete_data(char *id) {
//    std::vector<char *> memory_to_clear;

//    id = add_quote(id);
//    const char* base = "delete from login_data where id =";
//    char* temp = new char[strlen(base) + strlen(id)];
//    strncpy(temp, base, strlen(base));
//    strcpy(temp + strlen(base), id);

//    memory_to_clear.push_back(temp);
//    memory_to_clear.push_back(id);


//    result = this->execute_query(temp);
//    sites_to_vector();
//    dump(memory_to_clear);
//}

void DataBaseManager::delete_data(const char* site_name, const char* user_name){
    std::vector<char *> memory_to_clear;

    user_name = add_quote(user_name);
    site_name = add_quote(site_name);
    const char* base = "delete from login_data where site_name=";
    const char* base2 = " and user_name =";
    char* temp = new char[strlen(base) + strlen(site_name) + strlen(base2) + strlen(user_name)];

    size_t i = 0;
    strncpy(temp + i, base, strlen(base));
    i += strlen(base);
    strncpy(temp + i, site_name, strlen(site_name));
    i += strlen(site_name);
    strncpy(temp + i, base2, strlen(base2));
    i += strlen(base2);
    strcpy(temp + i, user_name);

    memory_to_clear.push_back(temp);

    sites_names.erase(std::remove(sites_names.begin(), sites_names.end(), site_name), sites_names.end());

    delete []site_name;
    delete []user_name;
    result = this->execute_query(temp);

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
    std::vector<char *> memory_to_clear;

    auto *res = new user_data;

    site_name = add_quote(site_name);
    const char* base = "select * from login_data where site_name =";
    char* temp = new char[strlen(base) + strlen(site_name)];
    strncpy(temp, base, strlen(base));
    strcpy(temp + strlen(base), site_name);
    memory_to_clear.push_back(temp);

    result = this->execute_query(temp);

    auto row = mysql_fetch_row(result);

    res->site_name = row[1];
    res->email = row[2];
    res->username = row[3];
    res->password = row[4];


    dump(memory_to_clear);
//    delete site_name;

    return *res;


}


void DataBaseManager::dump(std::vector<char *> mem) {
    mem.clear();
}




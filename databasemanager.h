#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <memory>
#include <mysql.h>
#include <vector>

struct connection_details{
    const char *server, *user, *password, *database;
};
struct user_data{
    char *site_name, *email, *username, *passowrd;
};

class DataBaseManager {
private:
    struct connection_details *connection_details;
    MYSQL *connection;
    MYSQL_RES *result;
    std::vector<char *> sites_names;
    static char* add_quote(const char* text);

public:
    DataBaseManager(struct connection_details *c_details);
    ~DataBaseManager();
    MYSQL *connection_setup();
    MYSQL_RES *execute_query(const char *sql_query);
    void select_row(char* id);
    void select_all();
    void new_data(struct user_data *userData);
    void delete_data(char* id);
    void showResult();
    void sites_to_vector();
    void show_sites();
    user_data &gather_info(const char* site_name);

};

#endif // DATABASEMANAGER_H

#ifndef PASSWORDCODER_H
#define PASSWORDCODER_H

#include <QString>

class PasswordCoder
{
private:
    int key;
public:
    PasswordCoder(int k);
    char *decode(QString txt);
    char *encode(QString txt);
    char *to_charPtr(QString text);
    char *generate_password();

};

#endif // PASSWORDCODER_H

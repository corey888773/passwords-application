#include "passwordcoder.h"
const char alphanum[] = "0123456789!@#$%^&*abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

PasswordCoder::PasswordCoder(int k) :key(k)
{

}
char *PasswordCoder::decode(QString txt){
    auto text = to_charPtr(txt);
    for (int i = 0; i < strlen(text); i++)
        text[i] -= key;
    return text;
}

char *PasswordCoder::encode(QString txt){
    auto text = to_charPtr(txt);

    for (int i = 0; i < strlen(text); i++)
        text[i] += key;
    return text;
}

char *PasswordCoder::to_charPtr(QString text){
    auto str = text.toStdString();
    char *cstr = new char[str.length() + 1];
    strcpy(cstr, str.c_str());
    return cstr;

}

char *PasswordCoder::generate_password(){
    char* generated = new char[16];
    for(int i = 0; i < 16; i++){
        generated[i] = alphanum[rand() % strlen(alphanum)];
    }

    return generated;

}

#ifndef SQLITEWRAPPER_H
#define SQLITEWRAPPER_H
#include <iostream>
#include "src/sqlite3.h"


class SQLiteWrapper
{
private:
#define SQL_STRING_EASY(...) #__VA_ARGS__

    const char* PATH_SQLITE_DATABASE = "AIWareHouse.db"; // programın çalıştığı konuma "WMS_1904040027.db" adında sqlite db'i
public:
    sqlite3* db;
    

    SQLiteWrapper();
    bool Olustur();
    bool Ac();
    bool Kapat();
};
//extern SQLiteWrapper sqlwrapper; // classı bir kere üretip, başka filelardan aynı classa erişebilmek için
#endif // SQLITEWRAPPER_H

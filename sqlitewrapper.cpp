#include "sqlitewrapper.h"

SQLiteWrapper::SQLiteWrapper()
{

}

bool SQLiteWrapper::Olustur(){
    sqlite3* db = NULL;
    sqlite3_stmt* query = NULL;
    int ret = 0;
    if (SQLITE_OK != (ret = sqlite3_initialize()))
    {
        return false;
    }
    if (SQLITE_OK != (ret = sqlite3_open_v2(PATH_SQLITE_DATABASE, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)))
    {
        return false;
    }
    //string createQuery = "CREATE TABLE IF NOT EXISTS items (userid INTEGER PRIMARY KEY, ipaddr TEXT,username TEXT,useradd TEXT,userphone INTEGER,age INTEGER, time TEXT NOT NULL DEFAULT (NOW()));";
    std::string createQuery = SQL_STRING_EASY(CREATE TABLE IF NOT EXISTS "Kullanicilar" (
                                 "ID"	INTEGER,
                                 "KullaniciAdi"	TEXT NOT NULL UNIQUE,
                                 "Sifre"	TEXT NOT NULL,
                                 "Ad"	TEXT,
                                "Soyad"	TEXT,
                                 PRIMARY KEY("ID" AUTOINCREMENT)
                             );); // SQL_STRING_EASY SAYESİNDE Tırnak karakterlerinin her birinin başına \ eklemek gerekmiyor, küçük bir trick
    sqlite3_stmt *createStmt;

    sqlite3_prepare(db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
    if (sqlite3_step(createStmt) != SQLITE_DONE)
        return false;


    if (NULL != query) sqlite3_finalize(query);
    if (NULL != db) sqlite3_close(db);
    sqlite3_shutdown();
    return true;
}
bool SQLiteWrapper::Ac(){
    return sqlite3_open(PATH_SQLITE_DATABASE,&db) == SQLITE_OK; // sonuç SQLITE_OK ise sorunsuz bir şekilde açılmış demektir.
}

bool SQLiteWrapper::Kapat(){
    return sqlite3_close(db) == SQLITE_OK; // sonuç SQLITE_OK ise sorunsuz bir şekilde kapanmış demektir.
}

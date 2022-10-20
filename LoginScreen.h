#ifndef LOGIN_SCREEN_H
#define LOGIN_SCREEN_H
#pragma once
#include "ui_LoginScreen.h"
#include "HousePricePrediction.h"
#include <iostream>
#include "sqlitewrapper.h"
#include <qmessagebox.h>
#include <qtimer.h>
enum AktifEkran {
	LOGIN,
	REGISTER
};

class LoginScreen : public QMainWindow
{
	Q_OBJECT

public:
	LoginScreen(QWidget* parent = Q_NULLPTR);
private:
	SQLiteWrapper sqlwrapper;
	AktifEkran aktifEkran;;
	void SinyalleriBagla();
	void Temizle();
	void KayitOl();
	void Hakkimizda();
	void GirisYap();

	void msgUyari(const char* msg);
	void msgHata(const char* msg);

	void IslemYap();
	Ui::LoginScreen ui;
};
#endif


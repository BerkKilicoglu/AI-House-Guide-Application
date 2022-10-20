#include "LoginScreen.h"

LoginScreen::LoginScreen(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.lblTitle->setText("<span style='font-size:14pt;'>Price Prediction App</span>");
	ui.txtKullaniciAdi->setVisible(false);
	ui.txtSifre->setVisible(false);
	ui.txtAd->setVisible(false);
	ui.txtSoyad->setVisible(false);
	ui.btnIslemiYap->setVisible(false);


	resize(QSize(300, 100));
	SinyalleriBagla();
	
	sqlwrapper.Olustur();
}

void LoginScreen::SinyalleriBagla() {
	connect(ui.btnKayitOl, &QPushButton::clicked, this, &LoginScreen::KayitOl);
	connect(ui.btnHakkimizda, &QPushButton::clicked, this, &LoginScreen::Hakkimizda);
	connect(ui.btnGiris, &QPushButton::clicked, this, &LoginScreen::GirisYap);
	connect(ui.btnIslemiYap, &QPushButton::clicked, this, &LoginScreen::IslemYap);
}

void LoginScreen::KayitOl() {
	Temizle();
	ui.txtKullaniciAdi->setVisible(true);
	ui.txtSifre->setVisible(true);
	ui.txtAd->setVisible(true);
	ui.txtSoyad->setVisible(true);
	ui.btnIslemiYap->setText(u8"Kayýt Ol");
	ui.btnIslemiYap->setVisible(true);
	ui.txtKullaniciAdi->setFocus();
	aktifEkran = AktifEkran::REGISTER;

}
void LoginScreen::Hakkimizda() {
	QPalette newPal(qApp->palette());
	newPal.setColor(QPalette::Link, Qt::white);
	newPal.setColor(QPalette::LinkVisited, Qt::white);
	qApp->setPalette(newPal);
	QMessageBox msgBox(this);
	msgBox.setWindowTitle(u8"Hakkýmýzda");
	msgBox.setStyleSheet("QWidget{}QMainWindow{}QDialog{}");
	msgBox.setTextFormat(Qt::RichText);   //this is what makes the links clickable
	msgBox.setText(QString::fromStdString(std::string(u8"<span style='font-size:13pt;'>Emre Demiran<br><a href='https://www.github.com/emrecpp'>Github</a><br><br><br>Berk Kýlýçoðlu<br><a href='https://github.com/BerkKilicoglu'>Github</a></span>")));
	msgBox.setStandardButtons(QMessageBox::Ok);

	msgBox.exec();

	//QMessageBox::information(this, "Hakkýmýzda", );
}
void LoginScreen::GirisYap() {
	Temizle();
	ui.txtKullaniciAdi->setVisible(true);
	ui.txtSifre->setVisible(true);
	ui.txtAd->setVisible(false);
	ui.txtSoyad->setVisible(false);
	ui.btnIslemiYap->setText(u8"Giriþ Yap");
	ui.btnIslemiYap->setVisible(true);
	ui.txtKullaniciAdi->setFocus();
	aktifEkran = AktifEkran::LOGIN;
}
void LoginScreen::Temizle() {
	ui.txtKullaniciAdi->clear();
	ui.txtSifre->clear();
	ui.txtAd->clear();
	ui.txtSoyad->clear();
}
void LoginScreen::msgUyari(const char* mesaj) {
	QMessageBox::information(this, u8"Uyarý", mesaj); // Türkçe karakterlerin düzgün gözükebilmesi için const char* ýn baþýna u8 ekleniyor
}
void LoginScreen::msgHata(const char* mesaj) {
	QMessageBox::critical(this, u8"Hata", mesaj); // Türkçe karakterlerin düzgün gözükebilmesi için const char* ýn baþýna u8 ekleniyor
}

void LoginScreen::IslemYap() {
	if (aktifEkran == AktifEkran::LOGIN) {
		if (!sqlwrapper.Ac())
			return;
		sqlite3_stmt* smt;

		std::string query = "SELECT Ad,Soyad FROM Kullanicilar WHERE KullaniciAdi=? AND Sifre=?"; // parametre(bind) kullanýyorum sql injection olmamasý için (güvenlik)

		if (sqlite3_prepare_v2(sqlwrapper.db, query.c_str(), query.length(), &smt, 0) != SQLITE_OK)
			return;


		sqlite3_bind_text(smt, 1, ui.txtKullaniciAdi->toPlainText().toStdString().c_str(), ui.txtKullaniciAdi->toPlainText().toStdString().length(), SQLITE_TRANSIENT);
		sqlite3_bind_text(smt, 2, ui.txtSifre->toPlainText().toStdString().c_str(), ui.txtSifre->toPlainText().toStdString().length(), SQLITE_TRANSIENT);



		auto rc = sqlite3_step(smt);
		if (rc != SQLITE_ROW)
		{
			sqlite3_finalize(smt);
			sqlwrapper.Kapat();
			msgHata(u8"Böyle bir kullanýcý bulunamadý yada þifre yanlýþ!");
			return;
		}
		if (rc != SQLITE_DONE && rc != SQLITE_OK)
		{
			const char* Ad = (const char*)sqlite3_column_text(smt, 0);
			const char* Soyad = (const char*)sqlite3_column_text(smt, 1);
			ui.lblBilgi->setText(QString::fromStdString(std::string(u8"<span style='font-size:16pt;'>Hoþgeldiniz, ") + std::string(Ad) + " " + std::string(Soyad) + "!</span>"));

			auto lambdaFunc = [&]() {
				hide();
				HousePricePrediction *w = new HousePricePrediction();
				w->show();
			};

			QTimer::singleShot(3000, lambdaFunc); // 3 sn bekledikten sonra diðer form açýlmasý için, timer kullanýlmasýnýn sebebi arayüzü bloklamamasý için (gui freeze olmamasý için)
		}

		sqlite3_finalize(smt);
		sqlwrapper.Kapat();
	}
	else if (aktifEkran == AktifEkran::REGISTER) {
		if (!sqlwrapper.Ac())
			return;
		std::string query = "SELECT count(*) FROM Kullanicilar WHERE KullaniciAdi=? LIMIT 1"; // parametre(bind) kullanýyorum sql injection olmamasý için (güvenlik)
		sqlite3_stmt* smt;

		if (sqlite3_prepare_v2(sqlwrapper.db, query.c_str(), query.length(), &smt, 0) != SQLITE_OK)
			return;

		sqlite3_bind_text(smt, 1, ui.txtKullaniciAdi->toPlainText().toStdString().c_str(), ui.txtKullaniciAdi->toPlainText().toStdString().length(), SQLITE_TRANSIENT);

		int rc = sqlite3_step(smt);
		int MevcutHesapSayisi = 0;  // Üye olurken bu üyelik varsa üyelik iþlemi yapýlmayacak
		if (rc != SQLITE_DONE && rc != SQLITE_OK)
		{
			MevcutHesapSayisi = sqlite3_column_int(smt, 0);
		}

		if (MevcutHesapSayisi > 0)
		{
			sqlite3_finalize(smt);
			sqlwrapper.Kapat();
			msgHata(u8"Bu kullanýcý adý daha önce alýnmýþ, lütfen baþka bir kullanýcý adý seçin");
			return;
		}
		sqlite3_finalize(smt);


		sqlite3_stmt* smt2;

		query = "INSERT INTO Kullanicilar (KullaniciAdi, Sifre, Ad,Soyad) VALUES (?,?,?,?)"; // parametre(bind) kullanýyorum sql injection olmamasý için (güvenlik)		
		if (sqlite3_prepare_v2(sqlwrapper.db, query.c_str(), query.length(), &smt2, 0) != SQLITE_OK)		
			return;
		

		sqlite3_bind_text(smt2, 1, ui.txtKullaniciAdi->toPlainText().toStdString().c_str(), ui.txtKullaniciAdi->toPlainText().toStdString().length(), SQLITE_TRANSIENT);
		sqlite3_bind_text(smt2, 2, ui.txtSifre->toPlainText().toStdString().c_str(), ui.txtSifre->toPlainText().toStdString().length(), SQLITE_TRANSIENT);
		sqlite3_bind_text(smt2, 3, ui.txtAd->toPlainText().toStdString().c_str(), ui.txtAd->toPlainText().toStdString().length(), SQLITE_TRANSIENT);
		sqlite3_bind_text(smt2, 4, ui.txtSoyad->toPlainText().toStdString().c_str(), ui.txtSoyad->toPlainText().toStdString().length(), SQLITE_TRANSIENT);

		rc = sqlite3_step(smt2);
		sqlite3_finalize(smt2);

		sqlwrapper.Kapat();// iþlem bitince veritabanýný kapatmasý için

		if (rc == SQLITE_DONE) {
			msgUyari(u8"Baþarýyla üye oldunuz!");
			Temizle();
			GirisYap();
		}
		else
			msgHata(u8"Üye olurken bir sorun oluþtu!");
	}
}
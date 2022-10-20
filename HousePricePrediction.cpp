#include "HousePricePrediction.h"
int HousePricePrediction::GetMahalleIndex(std::string Mahalle) // PKL dosyasýnda arama yapabilmek için stringi bir sayiya cevirmek lazim
{
	if (Mahalle == "Bluestem")
		return 1;
	if (Mahalle == "Brookside")
		return 3;
	if (Mahalle == "Crawfor")
		return 6;
	if (Mahalle == "Edwards")
		return 7;
	if (Mahalle == "Gilbert")
		return 8;
	if (Mahalle == "Mitchel")
		return 11;
	if (Mahalle == "Old Town")
		return 17;
	if (Mahalle == "SWISU")
		return 18;
	if (Mahalle == "Sawyer")
		return 19;
	if (Mahalle == "Somerset")
		return 21;
	if (Mahalle == "Timber")
		return 23;
	if (Mahalle == "Veenker")
		return 24;
	return 1;
}
void HousePricePrediction::Hesapla() {
	 PyObject* pName, * pModule, * pFunc; // python ile haberleþebilmesi için gerekli objeler
	 PyObject* pArgs, * pValue, * pValue2;
	int i;
	Py_Initialize(); // python initialize ediliyor


	pName = PyUnicode_DecodeFSDefault("predict_fnct"); // python kod dosyasýnýn adý
	pModule = PyImport_Import(pName);
	Py_DECREF(pName);

	if (pModule != NULL) {
		pFunc = PyObject_GetAttrString(pModule, "PredictHomeValue"); // fonksiyon adý

		if (pFunc && PyCallable_Check(pFunc)) {
			pArgs = PyTuple_New(11); // 11 tane parametre
			std::string CurPath = "";
			char buffer[MAX_PATH] = {0};
			GetModuleFileNameA(NULL, buffer, MAX_PATH);
			CurPath = std::string(buffer);
			CurPath = CurPath.substr(0, CurPath.find_last_of("\\/")) + "\\trained_model.pkl";
			int GarajM2 = 0, GarajAracSayisi = 0;
			if (ui.cmbGaraj->currentText().toStdString() == "Var")
			{
				GarajM2 = atoi(ui.txtGarajM2->toPlainText().toStdString().c_str());
				GarajAracSayisi = ui.spinGarajAracSayisi->value();
			}

			PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(CurPath.c_str()));
			PyTuple_SetItem(pArgs, 1, PyLong_FromLong(ui.spinBinaKalitesi->value()));
			PyTuple_SetItem(pArgs, 2, PyLong_FromLong(atoi(ui.txtNetM2->toPlainText().toStdString().c_str())));
			PyTuple_SetItem(pArgs, 3, PyLong_FromLong(GarajAracSayisi));
			PyTuple_SetItem(pArgs, 4, PyLong_FromLong(GarajM2));
			PyTuple_SetItem(pArgs, 5, PyLong_FromLong(ui.spinBanyoSayisi->value()));
			PyTuple_SetItem(pArgs, 6, PyLong_FromLong(ui.spinToplamOdaSayisi->value()));
			PyTuple_SetItem(pArgs, 7, PyLong_FromLong(ui.spinBinaYili->value()));
			PyTuple_SetItem(pArgs, 8, PyLong_FromLong(atoi(ui.txtBrutM2->toPlainText().toStdString().c_str())));
			int SomineSayisi = 0;
			if (ui.chkSomine->isChecked())
				SomineSayisi = ui.spinSomineSayisi->value();
			PyTuple_SetItem(pArgs, 9, PyLong_FromLong(SomineSayisi));
			PyTuple_SetItem(pArgs, 10, PyLong_FromLong(GetMahalleIndex(ui.cmbMahalle->currentText().toStdString())));
		}

		pValue = PyObject_CallObject(pFunc, pArgs);
		Py_DECREF(pArgs);
		if (pValue != NULL) {
			//const char* sonucstr = PyUnicode_AsUTF8(pValue);
			ui.lblSonuc->setVisible(true);
			float sonuc = PyLong_AsLong(pValue);
			std::string sonuc_str = std::to_string((int)sonuc);

			int n = sonuc_str.length() - 3;
			while (n > 0) {
				sonuc_str.insert(n, ",");
				n -= 3;
			}




			ui.lblSonuc->setText(QString::fromStdString(std::string("<span style='font-size:12pt;'>Tahmini Fiyat</span><br><span style='font-size:24pt;color:yellow;'>$"+ sonuc_str +"</span>")));
			//MessageBoxA(0, std::to_string(sonuc).c_str(), "Sonuç", MB_OK);
			//printf("Result of call: %ld\n", PyLong_AsLong(pValue));
			Py_DECREF(pValue);

		}
		else {
			Py_DECREF(pFunc);
			Py_DECREF(pModule);
			PyErr_Print();
			fprintf(stderr, "Call failed\n");
		}
	}
	else {
		if (PyErr_Occurred())
			PyErr_Print();
		

	}
	Py_DECREF(pFunc);
	Py_DECREF(pModule);
	//Py_Finalize();
}
HousePricePrediction::HousePricePrediction(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.txtBrutM2->setAlignment(Qt::AlignCenter);
	ui.txtNetM2->setAlignment(Qt::AlignCenter);
	ui.txtGarajM2->setAlignment(Qt::AlignCenter);
	ui.lblResim->setPixmap(QPixmap(":/Mahalle/Resimler/Mahalle/Bluestem.png"));
	ui.lblSonuc->setText("<span style='font-size:12pt;'>Tahmini Fiyat</span>");
	ui.lblSonuc->setVisible(false);


	ui.spinSomineSayisi->setVisible(false);
	ui.lblGaraj->setVisible(false);
	ui.txtGarajM2->setVisible(false);
	ui.lblGarajAracSayisi->setVisible(false);
	ui.spinGarajAracSayisi->setVisible(false);



	// Baþlangýç deðerleri
	ui.spinBinaKalitesi->setValue(1);
	ui.txtNetM2->setText("");
	ui.spinGarajAracSayisi->setValue(0);
	ui.txtGarajM2->setText("");
	ui.txtBrutM2->setText("");
	ui.spinBanyoSayisi->setValue(0);
	ui.spinToplamOdaSayisi->setValue(0);
	ui.spinBinaYili->setValue(2000);

	SinyalleriBagla();
}

void HousePricePrediction::SinyalleriBagla() {
	connect(ui.btnHesapla, &QPushButton::clicked, this, &HousePricePrediction::Hesapla);
	connect(ui.cmbMahalle, &QComboBox::currentTextChanged, this, &HousePricePrediction::MahalleDegisti);
	
	connect(ui.chkSomine, &QCheckBox::stateChanged, this, &HousePricePrediction::SomineTiklandi);
	connect(ui.cmbGaraj, &QComboBox::currentTextChanged, this, &HousePricePrediction::GarajDegisti);

}
void HousePricePrediction::MahalleDegisti(const QString& SecilenDeger)
{
	std::string Mahalle = SecilenDeger.toStdString();
	std::string TamKonum = ":/Mahalle/Resimler/Mahalle/" + Mahalle + ".png";
	ui.lblResim->setPixmap(QPixmap(TamKonum.c_str()));

}


void HousePricePrediction::SomineTiklandi(int State) {
	ui.spinSomineSayisi->setVisible(State > 0);
}

void HousePricePrediction::GarajDegisti(const QString& SecilenDeger) {
	bool visible = SecilenDeger.toStdString() ==  "Var";
	ui.spinSomineSayisi->setVisible(visible);
	ui.lblGaraj->setVisible(visible);
	ui.txtGarajM2->setVisible(visible);
	ui.lblGarajAracSayisi->setVisible(visible);
	ui.spinGarajAracSayisi->setVisible(visible);
}
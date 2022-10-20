#ifndef HOUSE_PRICE_PREDECTION_H
#define HOUSE_PRICE_PREDECTION_H
#pragma once
#ifdef _DEBUG
#undef _DEBUG
#undef slots // python.h ile qt slotlarý karisiyor o yüzden python include edilmeden önce undef yapýlmalý
#include <python.h>
#define _DEBUG
#else
#include <python.h>
#endif
#include <QtWidgets/QMainWindow>
#include <QtWidgets/qpushbutton.h>
#include "ui_HousePricePrediction.h"
#include <Windows.h>

class HousePricePrediction : public QMainWindow
{
    Q_OBJECT

public:
    HousePricePrediction(QWidget *parent = Q_NULLPTR);
    int GetMahalleIndex(std::string Mahalle);
private:
    Ui::HousePricePredictionClass ui;
    void SinyalleriBagla();
    void MahalleDegisti(const QString& SecilenDeger);
    void SomineTiklandi(int state);
    void GarajDegisti(const QString& SecilenDeger);



    void Hesapla();
};
#endif
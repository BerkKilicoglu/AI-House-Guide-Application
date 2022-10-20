#ifdef _DEBUG
#undef _DEBUG
#include <python.h>
#define _DEBUG
#else
#include <python.h>
#endif
#include "LoginScreen.h"
#include "HousePricePrediction.h"
#include <QtWidgets/QApplication>



#include <iostream>
#include <Windows.h>
#if 1
#pragma comment( linker, "/subsystem:windows" )
#else
#pragma comment( linker, "/subsystem:console" )
#endif
int main(int argc, char* argv[])
{
	//setbuf(stdout, NULL);
	QApplication a(argc, argv);
	a.setStyle("Fusion");
	LoginScreen w;

	w.show();
	return a.exec();
}


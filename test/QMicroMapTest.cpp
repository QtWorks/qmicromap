/*
 * QMicroMapTest.cpp
 *
 *  Created on: May 3, 2011
 *      Author: martinc
 */
#include "QMicroMapTest.h"
#include <iostream>
#include "QStationModelGraphicsItem.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
QMicroMapTest::QMicroMapTest(SpatiaLiteDB& db,
		double xmin,
		double ymin,
		double xmax,
		double ymax,
		std::string backgroundColor,
		QWidget* parent):
QDialog(parent),
_xmin(xmin),
_ymin(ymin),
_xmax(xmax),
_ymax(ymax),
_zoomInc(0.1)
{
	// setting up the Qt form
	setupUi(this);

	// we need a layout manager
	QVBoxLayout* vb = new QVBoxLayout(frame);

	// create the micromap and add to the layout
	_mm = new QMicroMap(db, _xmin, _ymin, _xmax, _ymax, backgroundColor);
	vb->addWidget(_mm);

	// connect signals
	connect(zoomIn,  SIGNAL(released()),        this, SLOT(zoomInSlot()));
	connect(zoomOut, SIGNAL(released()),        this, SLOT(zoomOutSlot()));
	connect(labels,  SIGNAL(stateChanged(int)), _mm,  SLOT(labels(int)));
	connect(grid,   SIGNAL(stateChanged(int)),  _mm,  SLOT(grid(int)));

	double wspd = 45;
	double wdir = 15;
	double lon = -86;
	double tdry = 23.0;
	double RH = 75.0;
	double presOrHeight = 1013;
	bool isPres = true;
	int hh = 17;
	int mm = 01;

	for (double lat = 24.0; lat < 29.0; lat += 0.8) {
		QStationModelGraphicsItem* sm
			= new QStationModelGraphicsItem(lon, lat, wspd, wdir, tdry, RH, presOrHeight, isPres, hh, mm, 60);
		wspd += 12;
		wdir += 13;
		lon -= 0.9;
		RH -= 4;
		mm += 3;
		tdry -= 1.3;
		presOrHeight -= 3;
		_mm->scene()->addItem(sm);
	}

	wspd = 55;
	wdir = 15;
	lon = -85;
	hh  -= 2;
	for (double lat = 27.0; lat >= 24.0; lat -= 0.51) {
		QStationModelGraphicsItem* sm
			= new QStationModelGraphicsItem(lon, lat, wspd, wdir, tdry, RH, presOrHeight, isPres, hh, mm, 60);
		wspd += 9;
		wdir += 13;
		lon -= 1.8;
		RH -= 4;
		mm -= 3;
		tdry += .8;
		presOrHeight -= 3;
		_mm->scene()->addItem(sm);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////
QMicroMapTest::~QMicroMapTest() {

}

/////////////////////////////////////////////////////////////////////////////////////////////////
void QMicroMapTest::zoomInSlot() {
	_mm->scale(1.0+_zoomInc, 1.0+_zoomInc);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void QMicroMapTest::zoomOutSlot() {
	_mm->scale(1.0/(1.0+_zoomInc), 1.0/(1.0+_zoomInc));
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void QMicroMapTest::zoom(double deltax, double deltay) {

	_xmin = _xmin+deltax;
	_xmax = _xmax-deltax;
	_ymin = _ymin+deltay;
	_ymax = _ymax-deltay;
	_mm->drawFeatures(_xmin, _ymin, _xmax, _ymax);
}

#include "altitudeplot.h"

AltitudePlot::AltitudePlot(QObject *parent, QCustomPlot *plot) : QObject(parent)
{
    plot = plot;

}

AltitudePlot::~AltitudePlot()
{

}


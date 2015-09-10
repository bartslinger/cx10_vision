#include "altitudeplot.h"

AltitudePlot::AltitudePlot(QObject *parent, QCustomPlot *plot_ref) : QObject(parent),
    counter(0),
    target(0)
{
    plot = plot_ref;
    plot->addGraph();
    plot->xAxis->setRange(0,500);
    plot->yAxis->setRange(0,60);
    plot->xAxis->setLabel("Time [frames]");
    plot->yAxis->setLabel("Height [weird unit]");

    plot->addGraph();
    plot->yAxis2->setVisible(true);
    plot->yAxis2->setRange(00,260);
    plot->graph(1)->setPen(QPen(Qt::red));

    plot->addGraph(plot->xAxis, plot->yAxis2);
    plot->graph(2)->setPen(QPen(Qt::black));
}

AltitudePlot::~AltitudePlot()
{

}

void AltitudePlot::addDataPoint(double value, double control)
{
    counter++;
    plot->graph(0)->addData(counter, value);
    plot->graph(1)->addData(counter, target);
    plot->graph(2)->addData(counter, control);
    plot->xAxis->setRange(counter, 200, Qt::AlignRight);
    plot->replot();
}

void AltitudePlot::setTarget(double new_target)
{
    target = new_target;
}


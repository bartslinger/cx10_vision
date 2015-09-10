#ifndef ALTITUDEPLOT_H
#define ALTITUDEPLOT_H

#include <QObject>
#include "qcustomplot.h"

class AltitudePlot : public QObject
{
    Q_OBJECT
public:
    explicit AltitudePlot(QObject *parent = 0, QCustomPlot *plot_ref = NULL);
    ~AltitudePlot();

    void addDataPoint(double value, double control);
    void setTarget(double new_target);

    QCustomPlot *plot;

private:
    int counter;
    double target;

signals:

public slots:
};

#endif // ALTITUDEPLOT_H

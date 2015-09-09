#ifndef ALTITUDEPLOT_H
#define ALTITUDEPLOT_H

#include <QObject>
#include "qcustomplot.h"

class AltitudePlot : public QObject
{
    Q_OBJECT
public:
    explicit AltitudePlot(QObject *parent = 0, QCustomPlot *plot = NULL);
    ~AltitudePlot();

    QCustomPlot *plot;

signals:

public slots:
};

#endif // ALTITUDEPLOT_H

#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <QObject>

class DataLogger : public QObject
{
    Q_OBJECT
public:
    explicit DataLogger(QObject *parent = 0);
    ~DataLogger();

signals:

public slots:
};

#endif // DATALOGGER_H

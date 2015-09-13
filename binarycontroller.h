#ifndef BINARYCONTROLLER_H
#define BINARYCONTROLLER_H

#include <QObject>

class BinaryController : public QObject
{
    Q_OBJECT
public:
    explicit BinaryController(QObject *parent = 0);
    ~BinaryController();

    void setGains(int up, int down);
    void setUpValue(int up);
    void setDownValue(int down);
    double propagate(double target, double measurement);


private:
    int up_value;
    int down_value;

signals:

public slots:
};

#endif // BINARYCONTROLLER_H

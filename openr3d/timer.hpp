#ifndef TIMER_HPP
#define TIMER_HPP

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
#include <cmath>

class Timer : public QObject
{
    Q_OBJECT

    QTimer qTimer;
    QElapsedTimer qElapsedTimer;

public:

    Timer(float cap = 0.f, QObject* parent = NULL) : QObject(parent) {
        qTimer.setInterval(std::roundf(1000.f / cap));
        QObject::connect(&qTimer, SIGNAL(timeout()), this, SLOT(tick()));
    }

    void start() {
        qTimer.start();
        qElapsedTimer.start();
    }

    void start(float cap) {
        qTimer.start(std::roundf(1000.f / cap));
        qElapsedTimer.start();
    }

private slots:

    void tick() {
        emit timeout(qElapsedTimer.restart() / 1000.0f);
    }

signals:

    void timeout(float deltaTime);

};

#endif // TIMER_HPP

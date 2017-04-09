#ifndef TIMER_HPP
#define TIMER_HPP

#include <QObject>
#include <QTimer>
#include <cmath>
#include <chrono>
#include <iostream>

class Timer : public QObject
{
    Q_OBJECT

    QTimer qTimer;
    std::chrono::high_resolution_clock::time_point lastTickTime;

public:

    Timer(float cap = 0.f, QObject* parent = NULL) : QObject(parent) {
        if (cap != 0.f)
            qTimer.setInterval(std::roundf(1000.f / cap));
        QObject::connect(&qTimer, SIGNAL(timeout()), this, SLOT(tick()));
    }

    void start() {
        qTimer.start();
        lastTickTime = std::chrono::high_resolution_clock::now();
    }

    void start(float cap) {
        if (cap == 0.f)
            qTimer.start(0);
        else
            qTimer.start(std::roundf(1000.f / cap));
        lastTickTime = std::chrono::high_resolution_clock::now();
    }

private slots:

    void tick() {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastTickTime).count() / 1000000.f;
        lastTickTime = currentTime;

        std::cout << "deltaTime = " << deltaTime << std::endl;
        emit timeout(deltaTime);
    }

signals:

    void timeout(float deltaTime);

};

#endif // TIMER_HPP

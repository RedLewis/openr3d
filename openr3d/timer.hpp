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
    float lastDeltaTime = 0;
    std::chrono::high_resolution_clock::time_point lastTickTime;

public:

    Timer(float maxTicksPerSecond = 0.f, QObject* parent = NULL) : QObject(parent) {
        if (maxTicksPerSecond != 0.f)
            qTimer.setInterval(std::roundf(1000.f / maxTicksPerSecond));
        QObject::connect(&qTimer, SIGNAL(timeout()), this, SLOT(processTick()));
    }

    void start() {
        qTimer.start();
        lastTickTime = std::chrono::high_resolution_clock::now();
    }

    void start(float maxTicksPerSecond) {
        if (maxTicksPerSecond == 0.f)
            qTimer.start(0);
        else
            qTimer.start(std::roundf(1000.f / maxTicksPerSecond));
        lastTickTime = std::chrono::high_resolution_clock::now();
    }

    float getLastDeltaTime() const {
        return lastDeltaTime;
    }

private slots:

    void processTick() {
        auto currentTime = std::chrono::high_resolution_clock::now();
        lastDeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastTickTime).count() / 1000000.f;
        lastTickTime = currentTime;
        emit tick(lastDeltaTime);
    }

signals:

    void tick(float deltaTime);

};

#endif // TIMER_HPP

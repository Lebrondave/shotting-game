#ifndef BGMTHREAD_H
#define BGMTHREAD_H

#include <QObject>
#include <QSound>

class bgmThread : public QObject
{
    Q_OBJECT
public:
    explicit bgmThread(QObject *parent = nullptr);

    void play();
signals:

public slots:

    //void slot_stopbgm();
};

#endif // BGMTHREAD_H

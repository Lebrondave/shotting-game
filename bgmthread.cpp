#include "bgmthread.h"

bgmThread::bgmThread(QObject *parent) : QObject(parent)
{

}

void bgmThread::play()
{
    QSound::play(":/source/music/bgm.wav");
}

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QKeyEvent>
#include <QPainter>
#include <QVector>
#include <QTime>
#include <QCoreApplication>
#include <QThread>
#include "user.h"
#include "enemyfactory.h"
#include "bullet_derived.h"
#include "header.h"
#include "kits.h"
#include "bgmthread.h"

#include <QDebug>




namespace Ui {
class mainWidget;
}

class mainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit mainWidget(QWidget *parent = 0);
    ~mainWidget();

    void slot_changeclearnumlabel(int num);

private:
    Ui::mainWidget *ui;

    user *ur;

    int bulletTimeId;
    int enemyappearTimeId;
    int enemyshotTimeId;

    int paintTimeId;

    int usershotTimeId;

    enemyfactory factory;

    QVector<items *> itemsls;

    QVector<picture *> clear_picture;

    QString bgpath;


    int bgX;
    int bgY;
    int bgH;
    int bgW;
    int bgpace;

    bgmThread *bgmT;
    QThread *thread;



protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *);

signals:
    void addenemy(int); //enemyID
    void healthloss(int);
    void ehealthloss(int,int);
    void enemyfactoryshot(int);
    void getitems(int);
    void playbgm();

public slots:

    void slot_gameover();
    void slot_appearitems(int ,int);

private slots:
    void on_progressBar_valueChanged(int value);
};

void Sleep(unsigned int msec);


#endif // MAINWIDGET_H

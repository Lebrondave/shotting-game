#include "mainwidget.h"
#include "ui_mainwidget.h"

int HEIGHT = 0;
int WIDTH = 0;

void Sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

mainWidget::mainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainWidget),ur(new user(width()/2,height()-50))
{    
    bgpath = ":/source/image/layout/bg0.jpg";

    bgmT = new bgmThread();
    thread = new QThread(this);
    bgmT->moveToThread(thread);

    connect(this ,&mainWidget::playbgm,bgmT,&bgmThread::play);

    thread->start();
    emit playbgm();


    setMouseTracking(true);
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    ui->setupUi(this);
    setCursor(Qt::BlankCursor);



    showFullScreen();

    HEIGHT = height();
    WIDTH = width();

    connect(this,&mainWidget::healthloss,ur,&user::slot_healthloss);
    connect(ur,&user::gameover,this,&mainWidget::slot_gameover);
    connect(this,&mainWidget::ehealthloss,&factory,&enemyfactory::slot_ehealthloss);
    connect(this,&mainWidget::addenemy,&factory ,&enemyfactory::slot_addenemy);
    connect(this, &mainWidget::getitems,ur,&user::slot_getitems);
    connect(&factory,&enemyfactory::appearitems,this,&mainWidget::slot_appearitems);
    connect(ur,&user::healthbarRefresh,this,&mainWidget::on_progressBar_valueChanged);
    connect(ur,&user::changeclearnumlabel,this,&mainWidget::slot_changeclearnumlabel);

    bulletTimeId = this -> startTimer(100);
    enemyappearTimeId = this -> startTimer(10000);
    enemyshotTimeId = this -> startTimer(1800);
    usershotTimeId = this -> startTimer(400);
    paintTimeId = this -> startTimer(10);

    ui->progressBar->move(width()-30,height()-200);

    slot_changeclearnumlabel(ur->clearscreen_num);

    ui->label->move(width()-250,height()-100);

    bgH = 800*width()/520;
    bgW = width();

    bgX = 0;
    bgY = -bgH+height();


}

mainWidget::~mainWidget()
{
    delete ui;
}

void mainWidget::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();

    //qDebug()<<x<<","<<y;


    if(x <= ceil(ur->W/2.0))
        x = ceil(ur->W/2.0);
    if(x >= ceil(this->width()-ur->W/2.0))
        x = ceil(this->width()-ur->W/2.0);
    if(y <= ceil(ur->H/2.0))
        y = ceil(ur->H/2.0);
    if(y >= ceil(height()-ur->H/2.0))
        y = ceil(height()-ur->H/2.0);

    //QCursor::setPos();

    this -> ur->x = x-ceil(ur->W/2.0);
    this -> ur->y = y-ceil(ur->H/2.0);

}

void mainWidget::mousePressEvent(QMouseEvent *ev)
{
//    if(ev->button() == Qt::LeftButton)
//    {
//        for(int i = 0;i<ur.wp->num;i++)
//        {
//            if(ur.wp->bulletIDls[i] == items::bullet_nottrack_10)
//            {
//               this->itemsls.push_back(new bullet_nottrack_10(ur.x+ur.W/2+ur.wp->locate_x[i],ur.y+ur.wp->locate_y[i],ur.wp->run_x[i],ur.wp->run_y[i],items::hurtenemy));


//            }

//            //

//        }




//    }
    /*else*/ if(ev->button() == Qt::RightButton)   //右键 清屏大招？？
    {
        if( ur->clearscreen_num != 0)
        {
            clear_picture.push_back(new picture(0,height()-329,407,329,":/source/image/layout/123.png"));

            //
            QSoundEffect effect;

            effect.setSource(QUrl::fromLocalFile(":/source/music/clearscreen.wav"));

            effect.setLoopCount(QSoundEffect::Infinite);

            effect.setVolume(1.0f);

            effect.play();
            //


            for(int i=0;i<itemsls.size();i++)
            {
                if(itemsls[i]->flag == items::hurtuser || itemsls[i]->flag == items::hurtboth)
                {
                    delete itemsls[i];
                    itemsls.erase(itemsls.begin()+i);
                    i--;
                }
            }

            for(int i=0;i<factory.ls.size();i++)
            {
                emit ehealthloss(i,100);
            }

            Sleep(1500);

            delete clear_picture[0];
            clear_picture.erase(clear_picture.begin());

            ur->clearscreen_num--;

            slot_changeclearnumlabel(ur->clearscreen_num);

        }


    }
}

void mainWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
//    case Qt::Key_Up :

//        if(this->ur.y1 != 0)
//        {
//            this->ur.y1 -= ur.pace;
//            this->ur.y2 -= ur.pace;
//        }
//        break;

//    case Qt::Key_Down :
//        if(this->ur.y2 != height())
//        {
//            this->ur.y1 += ur.pace;
//            this->ur.y2 += ur.pace;
//        }
//        break;

//    case Qt::Key_Left :
//        if(this->ur.x1!=0)
//        {
//            this->ur.x1 -= ur.pace;
//            this->ur.x2 -= ur.pace;
//        }
//        break;

//    case Qt::Key_Right :
//        if(this->ur.x2!=width())
//        {
//            this->ur.x1 += ur.pace;
//            this->ur.x2 += ur.pace;
//        }
//        break;

//    case Qt::Key_X :
//       this->ur.shot();
    case Qt::Key_Escape :
        //出个弹窗
        exit(0);
        break;
    }
}

void mainWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);



    p.drawPixmap(bgX,bgY,bgW,bgH,QPixmap(bgpath)); //背景图待定

    p.drawPixmap(this->ur->x,this->ur->y,ur->W,ur->H,QPixmap(ur->imagepath));

    for(int i=0;i<factory.ls.size();i++)
    {
        p.drawPixmap( factory.ls[i]->x, factory.ls[i]->y, factory.ls[i]->W, factory.ls[i]->H,QPixmap( factory.ls[i]->imagepath));
    }

    for(int i=0;i<this->itemsls.size();i++)
    {
        p.drawPixmap(this->itemsls[i]->x,this->itemsls[i]->y,this->itemsls[i]->W,this->itemsls[i]->H,QPixmap(this->itemsls[i]->imagepath));
    }

    if(clear_picture.size()!=0)
    {
        for(auto i:clear_picture)
        {
            p.drawPixmap(i->x,i->y,i->w,i->h,QPixmap(i->Path));
        }
    }
}

void mainWidget::timerEvent(QTimerEvent *event)
{
    if(event ->timerId() == this ->bulletTimeId)
    {

        bgY ++;
        if(bgY >= 0)
        {
            bgY = 0;
        }

        for(int i=0;i<this->factory.ls.size();i++)
        {
            factory.ls[i]->x += factory.ls[i]->run_x*factory.ls[i]->pace;
            factory.ls[i]->y += factory.ls[i]->run_y*factory.ls[i]->pace;

            if(factory.ls[i]->x<0
                    ||
                    factory.ls[i]->x+factory.ls[i]->W>width()
                    ||
                    factory.ls[i]->y<0
                    ||
                    factory.ls[i]->y+factory.ls[i]->H>height())
            {
                delete factory.ls[i];
                factory.ls.erase(factory.ls.begin()+i);
                i--;
            }

//            for(int j=0;j<this->factory.ls[i]->wp->ls.size();j++)
//            {
//                factory.ls[i]->wp->ls[j]->x += factory.ls[i]->wp->ls[j]->run.x();
//                factory.ls[i]->wp->ls[j]->y += factory.ls[i]->wp->ls[j]->run.y();

//                if(factory.ls[i]->wp->ls[j]->x<0
//                        ||
//                        factory.ls[i]->wp->ls[j]->x+factory.ls[i]->wp->ls[j]->W>width()
//                        ||
//                        factory.ls[i]->wp->ls[j]->y<0
//                        ||
//                        factory.ls[i]->wp->ls[j]->y+factory.ls[i]->wp->ls[j]->H>height())
//                {
//                    delete factory.ls[i]->wp->ls[j];
//                    factory.ls[i]->wp->ls.erase(factory.ls[i]->wp->ls.begin()+j);
//                    j--;
//                }

//                if(factory.ls[i](factory.ls[i]->wp->ls[j]))

//            }

        }

        for(int i=0;i<(this->itemsls).size();i++)
        {
            itemsls[i]->x += itemsls[i]->run_x*itemsls[i]->pace;
            itemsls[i]->y += itemsls[i]->run_y*itemsls[i]->pace;
            if(itemsls[i]->y!=0 && itemsls[i]->y!=height()-ceil(itemsls[i]->H/2.0) && itemsls[i]->x!=0 && itemsls[i]->x != width()-ceil(itemsls[i]->W/2.0))
            {
                if(itemsls[i]->flag == items::tools)
                {
                    if(ur->crash(itemsls[i]))
                    {
                        emit getitems(itemsls[i]->itemsID);

                        delete itemsls[i];
                        itemsls.erase(itemsls.begin()+i);
                        i--;
                    }
                }
                else if(itemsls[i]->flag == items::hurtuser)
                {
                    if(ur->crash(itemsls[i]))
                    {
                        emit healthloss(dynamic_cast<bullet*>(itemsls[i])->damage);

                        delete itemsls[i];
                        itemsls.erase(itemsls.begin()+i);
                        i--;
                    }
                }
                else if(itemsls[i]->flag == items::hurtenemy)
                {
                    for(int j=0;j<factory.ls.size();j++)
                    {
                        if(factory.ls[j]->crash(itemsls[i]))
                        {
                            emit ehealthloss(j,dynamic_cast<bullet*>(itemsls[i])->damage);

                            delete itemsls[i];
                            itemsls.erase(itemsls.begin()+i);
                            i--;
                        }
                    }

                }
                else if(itemsls[i]->flag == items::hurtboth)
                {

                }
            }
            else
            {
                delete itemsls[i];
                itemsls.erase(itemsls.begin()+i);
                i--;
            }
        }
    }
    else if(event ->timerId() == this -> enemyappearTimeId)  //这部分需要修改 有着怎样的流程
    {

//        // /////////////////////////////////////////////////////////////////////////////////
        static int times = 1;



        if(times <= 5)
            for(int i=0;i<times;i++)
            {
                emit addenemy(enemy::enemy_littlebrother);
            }

        else if(times > 5 && times <= 10)
        {
            for(int i =0;i<10-times;i++)
            {
                emit addenemy(enemy::enemy_littlebrother);
            }
            for(int i =0;i<times-5;i++)
            {
                emit addenemy(enemy::enemy_secondbrother);
            }
        }
        else if(times > 10 && times <= 15)
        {
            for(int i =0;i<15-times;i++)
            {
                emit addenemy(enemy::enemy_secondbrother);
            }
            for(int i =0;i<times-10;i++)
            {
                emit addenemy(enemy::enemy_thirdbrother);
            }

        }
        else if(times > 15 && times <= 20)
        {
            for(int i =0;i<20-times;i++)
            {
                emit addenemy(enemy::enemy_thirdbrother);
            }
            for(int i =0;i<times-15;i++)
            {
                emit addenemy(enemy::enemy_4thbrother);
            }

        }
        else if(times > 20)
        {
            for(int i =0;i<4;i++)
            {
                emit addenemy(enemy::enemy_4thbrother);
            }
            emit addenemy(enemy::enemy_5thbrother);

            killTimer(enemyappearTimeId);

        }

        times++;
//        emit addenemy(enemy::enemy_4thbrother);
//        emit addenemy(enemy::enemy_5thbrother);
//        killTimer(enemyappearTimeId);



        // /////////////////////////////////////////////////////////////////////////////////
    }
    else if(event -> timerId() == this -> enemyshotTimeId)
    {
        for(int i=0;i<factory.ls.size();i++)
        {
            static int locate_flag = 0;

            for(int j=0;j<factory.ls[i]->wp->num;j++)
            {
                if(factory.ls[i]->wp->bulletIDls[j] == items::bullet_nottrack_10)
                {
                   this->itemsls.push_back(new bullet_nottrack_10(factory.ls[i]->x+factory.ls[i]->W/2+factory.ls[i]->wp->locate_x[j]-15,factory.ls[i]->y+factory.ls[i]->H+factory.ls[i]->wp->locate_y[j]-15,factory.ls[i]->wp->run_x[j],factory.ls[i]->wp->run_y[j],items::hurtuser));
                }
                if(factory.ls[i]->wp->bulletIDls[j] == items::bullet_track_10)
                {

                    int X = factory.ls[i]->x+factory.ls[i]->W/2+factory.ls[i]->wp->locate_x[j]-15;
                    int Y = factory.ls[i]->y+factory.ls[i]->H/2+factory.ls[i]->wp->locate_y[j]-15;
                    float L = sqrt((float)(ur->x+ur->W/2-X)*(ur->x+ur->W/2-X)+(ur->y+ur->H/2-Y)*(ur->y+ur->H/2-Y));
                    factory.ls[i]->wp->run_x[j] = (ur->x+ur->W/2-X)/L;
                    factory.ls[i]->wp->run_y[j] = (ur->y+ur->H/2-Y)/L;

                    X += locate_flag * factory.ls[i]->wp->run_x[j]*20;
                    Y += locate_flag * factory.ls[i]->wp->run_y[j]*20;

                   this->itemsls.push_back(new bullet_track_10(X,Y,factory.ls[i]->wp->run_x[j],factory.ls[i]->wp->run_y[j],items::hurtuser));
                }

                //



                locate_flag ++;

            }

            locate_flag = 0;
        }
    }
    else if(event -> timerId() == this -> usershotTimeId)
    {
            for(int i = 0;i<ur->wp->num;i++)
            {
                if(ur->wp->bulletIDls[i] == items::bullet_unottrack_10)
                {
                   this->itemsls.push_back(new bullet_unottrack_10(ur->x+ur->W/2+ur->wp->locate_x[i]-15,ur->y+ur->wp->locate_y[i]-15,ur->wp->run_x[i],ur->wp->run_y[i],items::hurtenemy));


                }

                //

            }


     }
    else if(event -> timerId() == this -> paintTimeId)
    {
        update();
    }
}

void mainWidget::slot_gameover()
{
    for(int i=0;i<clear_picture.size();)
    {
        delete clear_picture[i];
        clear_picture.erase(clear_picture.begin()+i);
    }

    for(int i=0;i<factory.ls.size();)
    {
        delete factory.ls[i];
        factory.ls.erase(factory.ls.begin()+i);
    }

    for(int i=0;i<itemsls.size();)
    {
        delete itemsls[i];
        itemsls.erase(itemsls.begin()+i);
    }

    delete ur;
    ur = new user(width()/2,height()-50);
    exit(0);
    //弹出一个什么界面
}

void mainWidget::slot_appearitems(int x,int y)
{
    int flag = qrand()%items::items_end;
    if(flag == items::items_healthrecover)
    {
        itemsls.push_back(new items_healthrecover(x,y,0,1,items::tools));
    }
    else if(flag == items::items_changeweapon)
    {
        itemsls.push_back(new items_changeweapon(x,y,0,1,items::tools));
    }
    else if(flag == items::items_addclearnum)
    {
        itemsls.push_back(new items_addclearnum(x,y,0,1,items::tools));
    }
}

void mainWidget::on_progressBar_valueChanged(int value)
{
    ui->progressBar->setValue(value);
}

void mainWidget::slot_changeclearnumlabel(int num)
{
    ui->label->setText(QString("<h1>NUM LEFT: %1</h1>").arg(num));
}

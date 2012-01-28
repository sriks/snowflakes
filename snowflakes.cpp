#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QTime>
#include <QIcon>
#include <QDebug>
#include "snowflakes.h"

const int KSnowflakesCreationInterval = 50;
const int KAnimationDuration          = 1000*8;
const int KOffset                     = 400;

Snowflakes::Snowflakes(QObject* aParent)
{
    QGraphicsScene* scene = new QGraphicsScene(this);
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHints(QPainter::Antialiasing);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Base,QBrush(QColor(0,0,0,0))); // transparent bkg
    setPalette(palette);
    // Make this view transparent
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground,true);
    setWindowTitle("Snowflakes");
    bool loaded = mSnowflakePixmap.load(":/images/snowflake.png");
    qDebug()<<loaded;
    if(loaded)
    {
        setWindowIcon(QIcon(mSnowflakePixmap));
#ifdef Q_OS_SYMBIAN
        QSize s(4,4);
#else
        QSize s(8,8);
#endif
        mSnowflakePixmap = mSnowflakePixmap.scaled(s,
                                                   Qt::IgnoreAspectRatio,
                                                   Qt::SmoothTransformation);
    }

    // resize to full screen
    QRect mScreenRect = QDesktopWidget().screenGeometry();
    resize(mScreenRect.size());
    scene->setSceneRect(mScreenRect);
    mDirectionOffset = ((QTime().currentTime().msec()) %2 == 0)?(KOffset):(-KOffset);
    qDebug()<<"diroffset:"<<mDirectionOffset;
    setFocusPolicy(Qt::StrongFocus);

    // add snowman
    QPixmap p(":/images/snowman.png");
    if(!p.isNull())
    {
        p = p.scaled(QSize(64,64),
                     Qt::IgnoreAspectRatio,
                     Qt::SmoothTransformation);
    }
    mSnowMan = new QGraphicsWidget;
    mSnowMan->setObjectName("snowman");
    mSnowManPixmapItem = new QGraphicsPixmapItem(p,mSnowMan);
    scene->addItem(mSnowMan);
    mSnowManPixmapItem->setToolTip("Click me to close!");
    mSnowManPixmapItem->setOpacity(0.8);
    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    animateSnowMan();
}

void Snowflakes::startSnowfall()
{
    mTimerId = startTimer(KSnowflakesCreationInterval);
}

void Snowflakes::stopSnowfall()
{
    killTimer(mTimerId);
}

void Snowflakes::animateSnowMan()
{
    QPropertyAnimation* anim = new QPropertyAnimation(mSnowMan,"pos",this);
    QPoint snowManPos = rect().bottomRight();
    snowManPos.setX(snowManPos.x() - mSnowManPixmapItem->pixmap().width());
    snowManPos.setY( QDesktopWidget().availableGeometry().height() - mSnowManPixmapItem->pixmap().height());
    QPoint startPoint(rect().width()-mSnowManPixmapItem->pixmap().width(),rect().height()/2);
    anim->setStartValue(startPoint);
    anim->setEndValue(snowManPos);
    anim->setDuration(4000);
    anim->setEasingCurve(QEasingCurve::OutBounce);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void Snowflakes::mousePressEvent(QMouseEvent *aEvent)
{
    if(mSnowManPixmapItem == scene()->itemAt(aEvent->posF()))
    {
        QMessageBox msg;
        msg.setText("Close Snowflakes?");
        msg.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
        if(QMessageBox::Ok == msg.exec())
        { close(); }
    }
}

void Snowflakes::timerEvent(QTimerEvent *aEvent)
{
    Q_UNUSED(aEvent);
    QGraphicsWidget* snowflake = createSnowflake();
    QPropertyAnimation* anim = new QPropertyAnimation(snowflake,"pos",this);
    this->scene()->addItem(snowflake);
    anim->targetObject()->setParent(anim); //to delete when animation is finished
    QPoint startPoint(generateX(),-5);
    QPoint endPoint(startPoint.x()+mDirectionOffset,size().height());
    anim->setStartValue(startPoint);
    anim->setEndValue(endPoint);
    anim->setDuration(KAnimationDuration);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

QGraphicsWidget* Snowflakes::createSnowflake()
{
    QGraphicsWidget* snowflakeWidget = new QGraphicsWidget;
    QGraphicsPixmapItem* snowflake = new QGraphicsPixmapItem(snowflakeWidget);
    snowflake->setPixmap(mSnowflakePixmap);
    return snowflakeWidget;
}

int Snowflakes::generateX()
{
    static QTime startTime = QTime().currentTime();
    srand(QTime().currentTime().msecsTo(startTime));
    int r = rand();
    return r%size().width();
}

// eof

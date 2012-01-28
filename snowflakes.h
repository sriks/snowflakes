#ifndef SNOWFLAKES_H
#define SNOWFLAKES_H

#include <QGraphicsView>

class QGraphicsWidget;
class Snowflakes : public QGraphicsView
{
    Q_OBJECT
public:
    Snowflakes(QObject* aParent=0);
    void startSnowfall();
    void stopSnowfall();
private:
    void mousePressEvent(QMouseEvent *aEvent);
    void timerEvent(QTimerEvent *aEvent);
    void animateSnowMan();
    QGraphicsWidget* createSnowflake();
    int generateX();

private:

    QGraphicsWidget* mSnowMan;
    QGraphicsPixmapItem* mSnowManPixmapItem; // non owning
    QPixmap mSnowflakePixmap;
    QRect mScreenRect;
    int mTimerId;
    int mDirectionOffset;
};

#endif // SNOWFLAKES_H

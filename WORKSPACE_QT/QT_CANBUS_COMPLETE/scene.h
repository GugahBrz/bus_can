#ifndef SCENE_H
#define SCENE_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QColorDialog>
#include <QDebug>
#include <QLCDNumber>

class Scene : public QGraphicsScene
{
    Q_OBJECT

public:
    Scene( qreal x, qreal y, qreal width, qreal height, QObject * parent);
    void actualiser(int,int);

private:
    QPen *pen;
    QBrush *brush;
    QGraphicsEllipseItem *ellipse;
    QRadialGradient *gradient;
};

#endif // SCENE_H

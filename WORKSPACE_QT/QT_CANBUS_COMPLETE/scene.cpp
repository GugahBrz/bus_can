#include "scene.h"

Scene::Scene(qreal x, qreal y, qreal width, qreal height, QObject * parent) : QGraphicsScene(x, y, width, height, parent)
{
    ellipse = this->addEllipse(x,y,width,height);
    gradient=new QRadialGradient(50, 50, 50, 50, 50);
    gradient->setCenter(width/2,height/2);
    gradient->setFocalPoint(width/2,height/2);
    gradient->setSpread(QGradient::RepeatSpread);
    gradient->setColorAt(0, QColor(0,0,0));
    gradient->setColorAt(1, QColor(100,100,100));
    QBrush Brush_(*gradient);
    pen = new QPen();
    pen->setWidth(2);
    ellipse->setBrush(Brush_);
    ellipse->setPen(*pen);
}

void Scene::actualiser(int data1, int data0){
    int auxMax = 1840;  // maximo valor observado de luminicidad
    int result = (data1<<8) + data0;
    //int result = data1 << 8 + data0;
    int aux = (int)(((double)result/(double)auxMax)*255);
    gradient->setColorAt(0, QColor(aux,aux,0));
    QBrush Brush_(*gradient);
    ellipse->setBrush(Brush_);
}

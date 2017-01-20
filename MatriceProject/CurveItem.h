#ifndef CURVEITEM_H
#define CURVEITEM_H

#include <QPointF>
#include <QList>
#include <QGraphicsPathItem>

class CurveItem
{
public:
    CurveItem();

    QList<QPointF> getPoints();
    void setPoints(QList<QPointF> points);
    void addPoint(QPointF point);

    QGraphicsItemGroup* getItem();
    void setItem(QGraphicsItemGroup *item);

    QColor getColor();
    void setColor(QColor color);

    QString getName();
    void setName(QString name);

    int getSizePen();
    void setSizePen(int value);

    int size();

private:
    QList<QPointF> _points;
    QGraphicsItemGroup *_item;
    QColor _color;
    QString _name;
    int _size;
    bool _ini;


};

#endif // CURVEITEM_H

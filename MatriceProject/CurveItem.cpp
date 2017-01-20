#include "CurveItem.h"

CurveItem::CurveItem()
{
    _ini = false;
    _color = QColor(0,0,0);
    _name = "";
    _size = 1;
}

QList<QPointF> CurveItem::getPoints()
{
    return _points;
}

void CurveItem::setPoints(QList<QPointF> points)
{
    _ini = true;
    _points = QList<QPointF>(points);
}

void CurveItem::addPoint(QPointF point)
{
    if(! _ini)
    {
        _points = QList<QPointF>();
        _ini = true;
    }

    _points.push_back(point);
}

QGraphicsItemGroup *CurveItem::getItem()
{
    return _item;
}

void CurveItem::setItem(QGraphicsItemGroup *item)
{
    _item = item;
}

QColor CurveItem::getColor()
{
    return _color;
}

void CurveItem::setColor(QColor color)
{
    _color = color;
}

QString CurveItem::getName()
{
    return _name;
}

void CurveItem::setName(QString name)
{
    _name = name;
}

int CurveItem::getSizePen()
{
    return _size;
}

void CurveItem::setSizePen(int value)
{
    _size = value;
}

int CurveItem::size()
{
    return _points.size();
}

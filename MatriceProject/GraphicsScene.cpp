#include "GraphicsScene.h"

GraphicsScene::GraphicsScene(string name, QObject *parent) :
    QGraphicsScene(parent),
    Generator(name)
{
    this->setBackgroundBrush(Qt::gray);
    setSceneRect(0,0,600,500);

    _currentAction = Action::NONE;
    _currentColor = Qt::black;
    _isDraw = true;
    _isEdit = false;
    _backupStatus = false;
    _currentSizePen = 1;
    _selectedStroke = 0;
    _nbStrokeDraw = 0;
    _selectedTreeCurves = QList <CurveItem>();

}

GraphicsScene::~GraphicsScene()
{
    delete(_currentItem);
}

void GraphicsScene::addNewItem(QGraphicsItemGroup *item, QList<QPointF> points, QColor color, int sizePen)
{
    QGraphicsItemGroup *newItem = new QGraphicsItemGroup();
    addItem(newItem);
    newItem = item;

    CurveItem curveItem;
    curveItem.setItem(newItem);
    curveItem.setPoints(points);
    curveItem.setColor(color);
    curveItem.setSizePen(sizePen);

    stringstream ss;
    ss << "Curves" << _nbStrokeDraw;
    curveItem.setName(ss.str().c_str());

    _items.push_back(curveItem);
    _nbStrokeDraw++;

    emit newItemSignal(curveItem);
}

void GraphicsScene::changedName(QString oldName, QString newName)
{
    if(oldName == "")
        return;

    for(CurveItem &c : _items)
    {
        if(c.getName() == oldName)
        {
            c.setName(newName);
        }
    }
}


Node* GraphicsScene::clone(string) const
{
    return new GraphicsScene("QtGen");
}


bool GraphicsScene::start()
{
    cout << "Start Generator" << endl;
    return true;
}

bool GraphicsScene::stop()
{
    cout << "Stop Generator" << endl;
    return true;
}

bool GraphicsScene::generate(map<string,Group3D*>& groups3D,map<string,Group2D*>& groups2D,map<string,Group1D*>&,map<string,GroupSwitch*>&)
{
    OrientedPoint2D op(Point2D(_pointListe.last().x(),_pointListe.last().y()),0,0.0,0.0);
    updateData(_environment,groups2D,"QtTouch","QtTouch",to_string(0),"LG_ORIENTEDPOINT2D_TOUCH",_timestamp,op);

    return true;
}

set<string> GraphicsScene::produce() const
{
    set<string> produce;
    produce.insert(LG_ORIENTEDPOINT2D_TOUCH);
    produce.insert("LG_ORIENTEDPOINT2D_TOUCH");
    return produce;
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent * event)
{

    //  if(event->type() == QEvent::TabletPress)
    //      addEllipse(500, 200, 30, 30, QPen(_currentColor), QBrush(Qt::Dense5Pattern));


    if(event->button() & Qt::LeftButton)
    {

        _backupStatus = false;

        _pointListe = QList<QPointF>();
        double rad = 0.5;

        _currentItem = new QGraphicsItemGroup();
        addItem(_currentItem);

        QPointF pt = event->scenePos();
        _pointListe.push_back(QPointF(pt.x()-rad,pt.y()-rad));
        //     _currentItem->addToGroup(addEllipse(pt.x()-rad, pt.y()-rad, rad*2.0, rad*2.0,
        //                                          QPen(_currentColor), QBrush(Qt::Dense5Pattern)));
        _environment->update();
    }
    else
    {
        QPointF pt = event->scenePos();
        cout << "Pos : " << pt.x() << ", " << pt.y() << endl;
    }
}

void GraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    cout << "Double" << endl;
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{

    if(event->buttons() & Qt::LeftButton)
    {
        QPointF pt = event->scenePos();
        QPointF ptLast = _pointListe.last();
        _pointListe.push_back(QPointF(pt.x(),pt.y()));

        _currentItem->addToGroup(addLine(ptLast.x(),ptLast.y(),pt.x(), pt.y(),
                                         QPen(_currentColor,_currentSizePen)));
        _environment->update();
    }
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{

    if(event->button() & Qt::LeftButton)
    {
        _pointListe.push_back(QPointF(-1,-1));
        _environment->update();
        _pointListe.pop_back();

        mousePos = event->screenPos();

        if(_isDraw)
        {
            addNewItem(_currentItem, _pointListe, _currentColor, _currentSizePen);
        }
        else if(_isEdit)
        {
            _selectedPoints = QList<QPointF>();
            bool find = false;

            for(int i = _items.size()-1 ; i >= 0 ; i--)
            {
                bool col = _items[i].getItem()->collidesWithItem(_currentItem);
                bool findIntersect = false;
                if(col)
                {
                    _selectedCurve = i;
                    findIntersect = findIntersectionPoint(_items.at(i));
                    if(!find)
                        find = findIntersect;
                }
            }

            if(find)
                addNewItem(_currentItem, _pointListe, _currentColor, _currentSizePen);
            else
            {
                removeItem(_currentItem);
            }
        }
        else
        {
            switch(_currentAction)
            {
            case Action::CIRCLE:
                cout << "circle" << endl;
                break;
            case Action::X:
                cout << "X" << endl;
                break;
            case Action::STROKE:
            {
                for(int i = _items.size()-1 ; i >= 0 ; i--)
                {
                    QGraphicsItemGroup* curveItem =  _items[i].getItem();
                    bool col = curveItem->collidesWithItem(_currentItem);
                    if(col)
                    {
                        emit removeItemSignal(_items[i]);
                        removeItem(_items[i].getItem());
                        _items.removeAt(i);

                    }
                }
                cout << "Stroke" << endl;
                break;
            }
            case Action::NONE:
                cout << "Couldn't find a good match." << endl;
                break;
            }
            removeItem(_currentItem);
        }
    }
}

bool GraphicsScene::findIntersectionPoint(CurveItem curveItem)
{
    QList<QPointF> lCurve1 = curveItem.getPoints();
    QList<QPointF> lCurve2 = _pointListe;
    _intersectionPoints = new QGraphicsItemGroup();
    bool find = false;

    for(int i = 0 ; i < lCurve1.size()-1 ; i++)
    {
        for(int j = 0 ; j < lCurve2.size()-1 ; j++)
        {
            QPointF thePoint = intersection(lCurve1.at(i),lCurve1.at(i+1),lCurve2.at(j),lCurve2.at(j+1));
            if((thePoint.x() != -1) && (thePoint.y() != -1))
            {
                if(! _selectedPoints.isEmpty())
                {
                    if(! indexOf(_selectedPoints,thePoint))
                    {
                        _selectedPoints.push_back(thePoint);
                        _intersectionPoints->addToGroup(addEllipse(thePoint.x()-2, thePoint.y()-2, 3*2.0, 3*2.0,
                                                                   QPen(Qt::red), QBrush(Qt::red,Qt::SolidPattern)));
                    }
                }
                else
                {
                    _selectedPoints.push_back(thePoint);
                    _intersectionPoints->addToGroup(addEllipse(thePoint.x()-2, thePoint.y()-2, 3*2.0, 3*2.0,
                                                               QPen(Qt::red), QBrush(Qt::red,Qt::SolidPattern)));
                    find = true;
                }
            }
        }
    }

    if(!find)
        return false;

    addItem(_intersectionPoints);
    cutItem(_items[_selectedCurve].getPoints(), curveItem.getColor(), curveItem.getSizePen());
    removeItem(_items[_selectedCurve].getItem());

    for(int i = 0 ; i < _partStrokeList.size() ; i++)
    {
        if(i == _selectedStroke)
            _partStrokeList[i].setItem(createGraphicsCurve(_partStrokeList[i].getPoints(), Qt::green, _partStrokeList[i].getSizePen()));
        else
            _partStrokeList[i].setItem(createGraphicsCurve(_partStrokeList[i].getPoints(), _partStrokeList[i].getColor(), _partStrokeList[i].getSizePen()));

        addItem(_partStrokeList[i].getItem());
    }

    editChoice();
    return true;
}

QPointF GraphicsScene::intersection(QPointF p1,QPointF p2,QPointF p3,QPointF p4)
{
    float x1 = p1.x(), x2 = p2.x(), x3 = p3.x(), x4 = p4.x();
    float y1 = p1.y(), y2 = p2.y(), y3 = p3.y(), y4 = p4.y();

    float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (d == 0) return QPointF(-1,-1);

    float pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
    float x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
    float y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;

    if ( x < min(x1, x2) || x > max(x1, x2) ||
         x < min(x3, x4) || x > max(x3, x4) ) return QPointF(-1,-1);
    if ( y < min(y1, y2) || y > max(y1, y2) ||
         y < min(y3, y4) || y > max(y3, y4) ) return QPointF(-1,-1);

    QPointF ret;
    ret.setX(x);
    ret.setY(y);
    return ret;
}

void GraphicsScene::editChoice()
{
    popUp = new QDialog(0,Qt::FramelessWindowHint);
    layout = new QHBoxLayout();
    nextButton = new QPushButton("Next");
    okButton = new QPushButton("OK");

    layout->addWidget(nextButton);
    layout->addWidget(okButton);

    popUp->setLayout(layout);
    popUp->setGeometry(QRect(mousePos.x() ,mousePos.y(),0,0));
    popUp->setAttribute(Qt::WA_NoSystemBackground);
    popUp->setAttribute(Qt::WA_TranslucentBackground);

    QWidget::connect(nextButton,SIGNAL(clicked()),this,SLOT(switchStroke()));
    QWidget::connect(okButton,SIGNAL(clicked()),this,SLOT(deleteStroke()));

    popUp->exec();
}

void GraphicsScene::cutItem(QList<QPointF> liste, QColor color, int sizePen)
{
    int transitionPoint = 0;

    for(int p = 0 ; p < _selectedPoints.size() ; p++)
    {
        float distMin = sqrt(pow(liste.at(0).x() - _selectedPoints.at(p).x(),2) + pow(liste.at(0).y() - _selectedPoints.at(p).y(),2));
        int separationPoint = 1;

        for(int i = 1 ; i < liste.size() ; i++)
        {
            float newDist = sqrt(pow(liste.at(i).x() - _selectedPoints.at(p).x(),2) + pow(liste.at(i).y() - _selectedPoints.at(p).y(),2));
            if( distMin > newDist)
            {
                distMin = newDist;


                bool signX = (liste.at(i-1).x() - liste.at(i).x()) > 0;
                bool signY = (liste.at(i-1).y() - liste.at(i).y()) > 0;

                bool signXP = (liste.at(i).x() - _selectedPoints.at(p).x()) > 0;
                bool signYP = (liste.at(i).y() - _selectedPoints.at(p).y()) > 0;

                if((signX == signXP) && (signY == signYP))
                    separationPoint = i+1;
                else if((signX != signXP) && (signY != signYP))
                    separationPoint = i;
            }
        }

        liste.insert(separationPoint,_selectedPoints.at(p));

        CurveItem c;
        if(p != (_selectedPoints.size() -1))
        {
            c.setPoints(liste.mid(transitionPoint, (separationPoint +1) - transitionPoint));
            c.setColor(color);
            c.setSizePen(sizePen);
            _partStrokeList.push_back(c);
            transitionPoint = separationPoint;
        }
        else
        {
            c.setPoints(liste.mid(transitionPoint, (separationPoint +1) - transitionPoint));
            c.setColor(color);
            c.setSizePen(sizePen);
            _partStrokeList.push_back(c);
            transitionPoint = separationPoint;

            c.setPoints(liste.mid(transitionPoint,liste.size() - separationPoint));
            c.setColor(color);
            c.setSizePen(sizePen);
            _partStrokeList.push_back(c);
        }

    }
}

QGraphicsItemGroup* GraphicsScene::createGraphicsCurve(QList <QPointF> list, QColor color, int sizePen)
{
    QGraphicsItemGroup *newGraphicsCurve = new QGraphicsItemGroup();

    for(int i = 0 ; i < list.size() -1 ; i++)
    {
        newGraphicsCurve->addToGroup(addLine(list.at(i).x(), list.at(i).y(), list.at(i+1).x(), list.at(i+1).y(), QPen(color,sizePen)));
    }
    return newGraphicsCurve;
}

bool GraphicsScene::isDrawing()
{
    return _isDraw;
}

bool GraphicsScene::isEditing()
{
    return _isEdit;
}

void GraphicsScene::setCurrentAction(Action action)
{
    _currentAction = action;
}

void GraphicsScene::setColorPen(QColor color)
{
    _currentColor = color;
}

 void GraphicsScene::setSizePen(int value)
 {
     _currentSizePen = value;
 }


void GraphicsScene::setDraw()
{
    _isDraw = true;
    _isEdit = false;
}

void GraphicsScene::setEdit()
{
    _isDraw = false;
    _isEdit = true;
}

void GraphicsScene::setRecognize()
{
    _isDraw = false;
    _isEdit = false;
}

void GraphicsScene::switchStroke()
{
    _selectedStroke = (_selectedStroke + 1) % _partStrokeList.size();

    for(int i = 0 ; i < _partStrokeList.size() ; i++)
    {
        if(i == _selectedStroke)
            drawCurve(_partStrokeList[i],Qt::green, _partStrokeList[i].getSizePen());
        else
            drawCurve(_partStrokeList[i],_partStrokeList[i].getColor(), _partStrokeList[i].getSizePen());
    }

}

void GraphicsScene::setSelectedRow(QString name)
{
    if(_selectedTreeCurves.size() != 0)
    {
        for(CurveItem item : _selectedTreeCurves)
            drawCurve(item, item.getColor(), item.getSizePen());

        _selectedTreeCurves.clear();
    }

    CurveItem curve = findCurveItem(name);
    if(curve.getName() == "")
        return;

    drawCurve(curve, QColor(255,80,40), curve.getSizePen());
    _selectedTreeCurves.push_back(curve);
}

void GraphicsScene::setSelectedRows(QStringList names)
{
    if(_selectedTreeCurves.size() != 0)
    {
        for(CurveItem item : _selectedTreeCurves)
            drawCurve(item, item.getColor(), item.getSizePen());

        _selectedTreeCurves.clear();
    }

    for(QString name : names)
    {
        CurveItem curve = findCurveItem(name);
        if(curve.getName() == "")
            break;

        drawCurve(curve, QColor(255,80,40), curve.getSizePen());
        _selectedTreeCurves.push_back(curve);
    }
}


void GraphicsScene::deleteStroke()
{
    QList<QPointF> curveList;
    QGraphicsItemGroup *curveGraphics = new QGraphicsItemGroup();

    for(int i = 0 ; i < _partStrokeList.size() ; i++)
    {
        if(i != _selectedStroke)
        {
            for(int j = 0 ; j < _partStrokeList[i].getPoints().size() ; j++)
                curveList.push_back(_partStrokeList[i].getPoints().at(j));

            curveGraphics = createGraphicsCurve(curveList, _items[_selectedCurve].getColor(), _items[_selectedCurve].getSizePen());

            removeItem(_partStrokeList[i].getItem());
        }
        else
        {
            removeItem(_partStrokeList[i].getItem());
        }

    }

    _items[_selectedCurve].setPoints(curveList);
    _items[_selectedCurve].setItem(curveGraphics);
    addItem(_items[_selectedCurve].getItem());

    removeItem(_intersectionPoints);

    QWidget::disconnect(nextButton,SIGNAL(clicked()),this,SLOT(switchStroke()));
    QWidget::disconnect(okButton,SIGNAL(clicked()),this,SLOT(deleteStroke()));

    _selectedStroke = 0;
    _partStrokeList = QList <CurveItem>();
    _selectedPoints = QList <QPointF>();

    delete(_intersectionPoints);
    delete(nextButton);
    delete(okButton);
    delete(layout);

    popUp->close();
    delete(popUp);
}

bool GraphicsScene::indexOf(QList <QPointF> pointsList, QPointF point1)
{
    for(int p = 0; p < pointsList.size() ; p++)
    {
        if((pointsList.at(p).x() == point1.x()) && (pointsList.at(p).y() == point1.y()))
            return true;
    }

    return false;
}


QList <CurveItem> GraphicsScene::getItem()
{
    return _items;
}

void GraphicsScene::setItem(QList <CurveItem> list)
{
    _items = list;
}

bool GraphicsScene::isSaved()
{
    return _backupStatus;
}

void GraphicsScene::setBackUpStatus(bool status)
{
    _backupStatus = status;
}

CurveItem GraphicsScene::findCurveItem(QString name)
{
    for(CurveItem c : _items)
    {
        if(c.getName() == name)
        {
            return c;
        }
    }

    return CurveItem();
}

void GraphicsScene::drawCurve(CurveItem curveItem, QColor color, int sizePen)
{
    QList <QGraphicsItem *> l = curveItem.getItem()->childItems();

    for(QGraphicsItem * item : l)
    {
        ( qgraphicsitem_cast<QGraphicsLineItem *> (item))->setPen(QPen(color,sizePen));
    }
}

void GraphicsScene::drawAllCurves()
{
    for(int i = 0 ; i < _items.size() ; i++)
    {
        _items[i].setItem(createGraphicsCurve(_items[i].getPoints(), _items[i].getColor(), _items[i].getSizePen()));
        addItem(_items[i].getItem());
        emit newItemSignal(_items[i]);
    }

}

void GraphicsScene::RemoveAll()
{
    if(_items.size() == 0)
        return;

    for(int i = _items.size()-1 ; i >= 0 ; i--)
    {
        removeItem(_items[i].getItem());
        delete(_items[i].getItem());
        _items.removeAt(i);
    }
}

void GraphicsScene::ResetParameters()
{
    _selectedTreeCurves = QList <CurveItem>();
}

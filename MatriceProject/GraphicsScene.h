#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QPainterPath>
#include <QGraphicsView>
#include <iostream>
#include <QTouchEvent>

#include <QDialog>
#include <QPushButton>
#include <QLayout>
#include <QColor>

#include <QWaitCondition>
#include <QEventLoop>

#include "CurveItem.h"
#include "LgGenerator.h"

using namespace lg;

enum Action {NONE, DRAW, CIRCLE, X, STROKE};

class GraphicsScene : public QGraphicsScene, public Generator
{
    Q_OBJECT
public:
    GraphicsScene(string name, QObject* parent = 0);
    ~GraphicsScene();

    Node* clone(string) const;
    bool start();
    bool stop();

    bool generate(map<string,Group3D*>& groups3D,map<string,Group2D*>& groups2D,map<string,Group1D*>&,map<string,GroupSwitch*>&);

    set<string> produce() const;

    bool isDrawing();
    bool isEditing();

    bool findIntersectionPoint(CurveItem curveItem);
    QPointF intersection(QPointF p1,QPointF p2,QPointF p3,QPointF p4);
    void cutItem(QList<QPointF> liste, QColor color, int sizePen);
    void editChoice();

    QList <CurveItem> getItem();
    void setItem(QList <CurveItem> list);

    bool isSaved();
    void setBackUpStatus(bool status);

    void RemoveAll();
    void ResetParameters();

    QGraphicsItemGroup* createGraphicsCurve(QList <QPointF> list, QColor color, int sizePen);

    void drawCurve(CurveItem curveItem, QColor color, int sizePen);
    void drawAllCurves();

public slots:
    void setCurrentAction(Action action);
    void switchStroke();
    void deleteStroke();
    void setDraw();
    void setEdit();
    void setRecognize();

    void setColorPen(QColor color);
    void setSizePen(int value);

    void setSelectedRow(QString name);
    void setSelectedRows(QStringList names);
    void changedName(QString oldName, QString newName);


signals :
    void newItemSignal(CurveItem item);
    void removeItemSignal(CurveItem item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    void addNewItem(QGraphicsItemGroup *item, QList<QPointF> points, QColor color, int sizePen);
    bool indexOf(QList<QPointF> pointsList, QPointF point1);
    CurveItem findCurveItem(QString name);

    QList <CurveItem> _items;
    QList <QPointF> _pointListe;

    QList <CurveItem> _partStrokeList;
    int _selectedStroke;

    QGraphicsItemGroup *_currentItem;
    QGraphicsItemGroup *_intersectionPoints;
    QColor _currentColor;
    int _currentSizePen;
    Action _currentAction;
    bool _isDraw;
    bool _isEdit;

    int _selectedCurve;
    QList <QPointF> _selectedPoints;

    QList <CurveItem> _selectedTreeCurves;

    QPointF mousePos;

    QDialog* popUp;
    QHBoxLayout* layout;
    QPushButton* nextButton;
    QPushButton* okButton;

    bool _backupStatus;
    int _nbStrokeDraw;

};

#endif // GRAPHICSSCENE_H

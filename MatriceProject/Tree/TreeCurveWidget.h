#ifndef TREECURVEWIDGET_H
#define TREECURVEWIDGET_H

#include <QTreeView>
#include <QMouseEvent>
#include <iostream>

#include "TreeModel.h"
#include "CurveItem.h"


class TreeCurveWidget : public QTreeView
{
    Q_OBJECT
public:
    TreeCurveWidget(QList <CurveItem> *curves);
    QStringList extractStringsFromModel(QAbstractItemModel *model, const QModelIndex &parent);
    std::pair<QModelIndex, int> findRow(QString s, const QModelIndex &parent);
    TreeModel *_model;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

public slots :
    void insertRow(CurveItem item);
    void insertDirectory(QString name);
    void removeRow(CurveItem item);
    void removeAll(QList<CurveItem> l);

    void sendNewData(const QVariant &oldValue, const QVariant &value);

signals :
    void selectedRow(QString name);
    void selectedRows(QStringList names);
    void sendNewName(QString oldName,QString newName);

private:
    int _nbRow;
    int _nbDirectory;
    QList <CurveItem> *_curves;
    QModelIndex currentDirectory;


};

#endif // TREECURVEWIDGET_H

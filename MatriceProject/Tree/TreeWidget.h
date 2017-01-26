#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "TreeCurveWidget.h"

class TreeWidget : public QWidget
{
public:
    TreeWidget(QList<CurveItem> *curves);

    TreeCurveWidget *getTree();

private:
    TreeCurveWidget *_tree;
    QPushButton *_add;
    QPushButton *_delete;

    QHBoxLayout *_hLayout;
    QVBoxLayout *_vLayout;

};

#endif // TREEWIDGET_H

#include "TreeWidget.h"

TreeWidget::TreeWidget(QList<CurveItem> *curves): QWidget(0,Qt::FramelessWindowHint)
{

    _hLayout = new QHBoxLayout();

    _add = new QPushButton();
    _add->setStyleSheet(CSSWidget::getButtonStyleSheet(":/Resources/add.png"));
    _delete = new QPushButton();
    _delete->setStyleSheet(CSSWidget::getButtonStyleSheet(":/Resources/delete.png"));

    _hLayout->addWidget(_add);
    _hLayout->addWidget(_delete);

    _vLayout = new QVBoxLayout();

    _tree = new TreeCurveWidget(curves);

    _vLayout->addLayout(_hLayout);
    _vLayout->addWidget(_tree);

    setLayout(_vLayout);
}

TreeCurveWidget* TreeWidget::getTree()
{
    return _tree;
}
